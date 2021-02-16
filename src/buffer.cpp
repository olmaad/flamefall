#include "buffer.h"

BufferWrapper::BufferWrapper(GLenum type) :
    m_type(type)
{
    glGenBuffers(1, &m_id);
}

BufferWrapper::~BufferWrapper()
{
    glDeleteBuffers(1, &m_id);
}

GLuint BufferWrapper::getId() const
{
    return m_id;
}

void BufferWrapper::bind()
{
    glBindBuffer(m_type, m_id);
}

VertexAttribArrayLock::VertexAttribArrayLock(
    BufferWrapper& buffer,
    GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const void * offset) :
    m_index(index),
    m_size(size),
    m_type(type),
    m_normalized(normalized),
    m_stride(stride),
    m_offset(offset)
{
    buffer.bind();

    glEnableVertexAttribArray(m_index);
    glVertexAttribPointer(m_index, m_size, m_type, m_normalized, m_stride, m_offset);
}

VertexAttribArrayLock::~VertexAttribArrayLock()
{
    glDisableVertexAttribArray(m_index);
}
