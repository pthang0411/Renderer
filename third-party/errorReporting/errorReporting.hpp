#ifndef ERROR_REPORTING_HPP
#define ERROR_REPORTING_HPP

#include <glad/glad.h> // Include GLAD to get OpenGL function pointers
#include <iostream>    // For std::cout and std::cerr

// This is the callback function that OpenGL will use to report messages.
// It needs the GLAPIENTRY calling convention for compatibility.
void GLAPIENTRY glDebugOutput(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char* message,
                              const void* userParam)
{
    // Ignore common non-significant performance warnings (you can adjust these)
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204 || id == 131222) return;
    if (type == GL_DEBUG_TYPE_PERFORMANCE) return; // Ignore all performance messages

    std::cerr << "---------------" << std::endl;
    std::cerr << "Debug message (" << id << "): " << message << std::endl;

    std::cerr << "Source: ";
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cerr << "API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cerr << "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cerr << "Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cerr << "Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cerr << "Other"; break;
    }
    std::cerr << std::endl;

    std::cerr << "Type: ";
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cerr << "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cerr << "Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cerr << "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cerr << "Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cerr << "Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cerr << "Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cerr << "Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cerr << "Other"; break;
    }
    std::cerr << std::endl;

    std::cerr << "Severity: ";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cerr << "high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cerr << "medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cerr << "low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "notification"; break;
    }
    std::cerr << std::endl;
    std::cerr << std::endl;
}

// Function to enable the OpenGL debug output
void enableReportGlErrors()
{
    // Check if GL_DEBUG_OUTPUT is available (requires OpenGL 4.3 or ARB_debug_output extension)
    if (glad_glDebugMessageCallback) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Makes sure errors are reported immediately
        glDebugMessageCallback(glDebugOutput, nullptr); // Set your callback function
        // Optionally, filter messages. This line enables all messages.
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        // Note: The line `glDebugMessageCallback(0, nullptr);` at the end of your original `enableReportGlErrors`
        // would disable the callback immediately after setting it. You should remove it.
    } else {
        std::cerr << "Warning: OpenGL Debug Output not available on this system." << std::endl;
    }
}

#endif // ERROR_REPORTING_HPP