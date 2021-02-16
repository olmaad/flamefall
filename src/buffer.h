#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <gl/glew.h>

class BufferWrapper
{
public:
    BufferWrapper(GLenum type);
    BufferWrapper(const BufferWrapper&) = delete;
    BufferWrapper(BufferWrapper&&) = default;
    ~BufferWrapper();

    BufferWrapper& operator=(const BufferWrapper&) = delete;
    BufferWrapper& operator=(BufferWrapper&&) = default;

    GLuint getId() const;

    void bind();

    template<typename T>
    void setData(const std::vector<T> data)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    }

private:
    GLenum m_type;
    GLuint m_id;

};

class VertexAttribArrayLock
{
public:
    VertexAttribArrayLock(
        BufferWrapper& buffer,
        GLuint index,
        GLint size,
        GLenum type,
        GLboolean normalized,
        GLsizei stride,
        const void * pointer);
    VertexAttribArrayLock(const VertexAttribArrayLock&) = delete;
    VertexAttribArrayLock(VertexAttribArrayLock&&) = default;
    ~VertexAttribArrayLock();

    VertexAttribArrayLock& operator=(const VertexAttribArrayLock&) = delete;
    VertexAttribArrayLock& operator=(VertexAttribArrayLock&&) = default;

private:
    GLuint m_index;
    GLint m_size;
    GLenum m_type;
    GLboolean m_normalized;
    GLsizei m_stride;
    const void * m_offset;

};

#endif // !BUFFER_H
