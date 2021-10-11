#include "sane/model/model.hpp"

#include <fstream>
#include <string>

#include "sane/logging/log.hpp"

namespace
{
    typedef std::vector<std::string> Tokens;

    Tokens split(const std::string& s, char seperator)
    {
        Tokens tokens;
        std::string::size_type prev_pos = 0, pos = 0;
        while ((pos = s.find(seperator, pos)) != std::string::npos)
        {
            std::string substring(s.substr(prev_pos, pos - prev_pos));
            tokens.push_back(substring);
            prev_pos = ++pos;
        }

        tokens.push_back(s.substr(prev_pos, pos - prev_pos));
        return tokens;
    }

    static const char* vs_modern = R""(
    #version 330
    layout(location = 0) in vec3 vPos;
    uniform mat4 MVP;
    out vec4 color;
    void main() {
      gl_Position = MVP * vec4(vPos, 1.0);
      color = vec4(1, 1, 1, 1);
      color *= 15.f / (length(gl_Position) * length(gl_Position));
      color.a = 1.f;
    }
)"";

    static const char* fs_modern = R""(
    #version 330
    out vec4 outColor;
    in vec4 color;
    void main() {
      outColor = color;
    }
)"";
}

namespace Sane
{
    Model::Model(const char* path)
        : vertices_buffer(GL_ARRAY_BUFFER)
        , normals_buffer(GL_ARRAY_BUFFER)
        , uvs_buffer(GL_ARRAY_BUFFER)
        , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
        , sProg_(vs_modern, fs_modern)
        , vPos(sProg_.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0)
        , Listener("Model")
    {
        std::vector<VertexData::Position> vertices;
        std::vector<VertexData::Normal> normals;
        std::vector<VertexData::UV> uvs;

        std::ifstream ifs(path);
        if (ifs.is_open())
        {
            std::string line;
            while (getline(ifs, line))
            {
                Tokens tokens = split(line, ' ');
                if (tokens.size() > 3 && tokens.front().compare("v") == 0)
                {
                    vertices.push_back({ stof(tokens[1]), stof(tokens[2]), stof(tokens[3]) });
                }
                else if (tokens.size() > 3 && tokens.front().compare("vn") == 0)
                {
                    normals.push_back({ stof(tokens[1]), stof(tokens[2]), stof(tokens[3]) });
                }
                else if (tokens.size() > 2 && tokens.front().compare("vt") == 0)
                {
                    uvs.push_back({ stof(tokens[1]), stof(tokens[2]), 0 });
                }
                else if (tokens.size() > 3 && tokens.front().compare("f") == 0)
                {
                    Tokens vertInfo1 = split(tokens[1], '/');
                    Tokens vertInfo2 = split(tokens[2], '/');
                    Tokens vertInfo3 = split(tokens[3], '/');

                    //Tex Coords
                    if (vertInfo1.size() > 1 && strcmp(vertInfo1[1].c_str(), "") != 0 && uvs.size() > 0)
                    {
                        uvs_.push_back(uvs[stoi(vertInfo1[1]) - 1]);
                        uvs_.push_back(uvs[stoi(vertInfo2[1]) - 1]);
                        uvs_.push_back(uvs[stoi(vertInfo3[1]) - 1]);
                    }

                    //Normals
                    if (vertInfo1.size() > 2 && strcmp(vertInfo1[2].c_str(), "") != 0 && normals.size() > 0)
                    {
                        normals_.push_back(normals[stoi(vertInfo1[2]) - 1]);
                        normals_.push_back(normals[stoi(vertInfo2[2]) - 1]);
                        normals_.push_back(normals[stoi(vertInfo3[2]) - 1]);
                    }

                    if (vertInfo1.size() > 0)
                    {
                        vertices_.push_back(vertices[stoi(vertInfo1[0]) - 1]);
                        vertices_.back().x -= .5f;
                        vertices_.back().y -= .5f;
                        vertices_.back().z -= 5.f;
                        indices_.push_back(vertices_.size() - 1);

                        vertices_.push_back(vertices[stoi(vertInfo2[0]) - 1]);
                        vertices_.back().x -= .5f;
                        vertices_.back().y -= .5f;
                        vertices_.back().z -= 5.f;
                        indices_.push_back(vertices_.size() - 1);

                        vertices_.push_back(vertices[stoi(vertInfo3[0]) - 1]);
                        vertices_.back().x -= .5f;
                        vertices_.back().y -= .5f;
                        vertices_.back().z -= 5.f;
                        indices_.push_back(vertices_.size() - 1);
                    }
                }
                else
                {
                    SANE_INFO("Model class not using {} data", tokens.front());
                }
            }
            ifs.close();
        }
        else
        {
            SANE_ERROR("Failed to model: {}", path);
        }

        SANE_INFO("Loaded model: {}", path);

        {
            unsigned int reminder = vertices_.size() % 12;
            numVerticesPacked = (vertices_.size() + reminder) / 4;
            verticesPacked = (Vec3Packed*)_aligned_malloc(numVerticesPacked * sizeof(Vec3Packed), 16);
            unsigned int packIndex = 0;
            for (unsigned int i = 0; i < vertices_.size() + reminder; i += 12)
            {
                auto& v0 = vertices_[i];
                auto& v1 = vertices_[i + 1];
                auto& v2 = vertices_[i + 2];

                auto& v3 = vertices_[i + 3];
                auto& v4 = vertices_[i + 4];
                auto& v5 = vertices_[i + 5];

                auto& v6 = vertices_[i + 6];
                auto& v7 = vertices_[i + 7];
                auto& v8 = vertices_[i + 8];

                auto& v9 = vertices_[i + 9];
                auto& v10 = vertices_[i + 10];
                auto& v11 = vertices_[i + 11];

                verticesPacked[packIndex] = Vec3Packed(v0, v3, v6, v9);
                verticesPacked[packIndex + 1] = Vec3Packed(v1, v4, v7, v10);
                verticesPacked[packIndex + 2] = Vec3Packed(v2, v5, v8, v11);

                packIndex += 3;
            }
        }
    }

    bool Model::ProcessEvent(Event& evt)
    {
        if (evt.action == kMouseMoveEvent)
        {
            Input::MouseMoveEvent& mme = *(Input::MouseMoveEvent*)evt.data;
            x = (float)mme.xpos;
            y = (float)mme.ypos;
        }

        return false;
    }

    void Model::DrawImmediate()
    {
        static float xC = -1.f;
        static float yC = -1.f;
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(50.0f, 50.0f, 50.0f);
        glEnd();

        glm::mat4 m = glm::mat4(1.f);
        glm::mat4 p = glm::perspective(45.0f, 16.f / 9.f, 1.0f, 200.0f);
        glm::mat4 mvp = p * m;

        const Ray ray{ {xC, yC, 0}, {0, 0, -1.f} };
        float distance = std::numeric_limits<float>::max();
        unsigned int triangleIndex;
        float u;
        float v;
        bool intersection = intersect(verticesPacked, numVerticesPacked, ray, distance, triangleIndex, u, v);

        if (intersection)
        {
            SANE_INFO("Intersection: {},{} -- {}, {}, {}, {}", xC, yC, distance, triangleIndex, u, v);
        }
        xC += .5f;
        if (xC > 1.f)
        {
            xC = -1.f;
            yC += .5f;
        }

        sProg_.Bind();

        vertices_buffer.Bind();
        if (!buffered_)
            vertices_buffer.BufferData(sizeof(float) * vertices_.size() * 4, &vertices_[0], GL_STATIC_DRAW);
        vPos.Enable();

        indices_buffer.Bind();
        if (!buffered_)
            indices_buffer.BufferData(sizeof(uint32_t) * indices_.size(), &indices_[0], GL_STATIC_DRAW);

        glUniformMatrix4fv(sProg_.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);
        indices_buffer.Unbind();

        vertices_buffer.Unbind();
        sProg_.Unbind();

        buffered_ = true;
    }
}