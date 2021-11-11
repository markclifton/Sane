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
    uniform vec4 iColor;
    uniform mat4 MVP;
    out vec4 color;
    void main() {
      gl_Position = MVP * vec4(vPos, 1.0);
      color = iColor;

        vec3 origin = vec3(0,0,0);
        float test = clamp(distance(gl_Position.xyz, origin), 0, 10) / 10.f;

        color.xyz =  vec3(1,1,1); //1 - test, 1 - test, 1 - test);
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
        : Listener("Model")
        , sProg_(vs_modern, fs_modern)
        , vertices_buffer(GL_ARRAY_BUFFER)
        , normals_buffer(GL_ARRAY_BUFFER)
        , uvs_buffer(GL_ARRAY_BUFFER)
        , indices_buffer(GL_ELEMENT_ARRAY_BUFFER)
        , vPos(sProg_.GetAttribLocation("vPos"), 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0)
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
            return;
        }
    }

    bool Model::ProcessEvent(Event& evt)
    {
        return false;
    }

    void Model::DrawImmediate(glm::mat4 mvp)
    {
        glm::vec4 iColor(255, 255, 255, 255);

        sProg_.Bind();

        vertices_buffer.Bind();
        if (!buffered_)
            vertices_buffer.BufferData(sizeof(float) * vertices_.size() * 4, &vertices_[0], GL_STATIC_DRAW);
        vPos.Enable();

        indices_buffer.Bind();
        if (!buffered_)
            indices_buffer.BufferData(sizeof(uint32_t) * indices_.size(), &indices_[0], GL_STATIC_DRAW);

        glUniformMatrix4fv(sProg_.GetUniformLocaition("MVP"), 1, GL_FALSE, (const GLfloat*)&mvp[0][0]);
        glUniform4f(sProg_.GetUniformLocaition("iColor"), iColor.r, iColor.g, iColor.b, iColor.a);

        glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);
        indices_buffer.Unbind();

        vertices_buffer.Unbind();
        sProg_.Unbind();

        buffered_ = true;
    }
}