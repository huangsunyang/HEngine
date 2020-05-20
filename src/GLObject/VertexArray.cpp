#include "GLObject/VertexArray.hpp"

VertexArray::VertexArray() {
    glGenBuffers(1, &vao);
}

void VertexArray::activate() {
    glBindVertexArray(vao);
}