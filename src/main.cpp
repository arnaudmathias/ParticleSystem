#include "part.h"
#include "Shader.h"
#include "CLenv.h"
#include "Env.h"
#include "Camera.h"
#include "Scene.h"

int main(void)
{
    Env env(WIDTH, HEIGHT);

    Shader shader("shaders/part.fs.glsl", "shaders/part.vs.glsl");
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    CLenv cl("shaders/kernel.cl");
    cl.createBuffer();

    InputHandler inputHandler;
    Camera camera(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), WIDTH, HEIGHT);
	camera.inputHandler = &inputHandler;
    glfwSetWindowUserPointer(env.window, &inputHandler);
    glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(env.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(env.window, mouseCallback);
    glfwSetKeyCallback(env.window, keyCallback);
	//glfwSetCursorPos(env.window, (camera.width / 2.0), (camera.height / 2.0));

    Scene scene(&cl, &camera);

    bool anim = false;
	cl_float4 cursorpos;
    cursorpos.s[0] = 0.0f;
    cursorpos.s[1] = 0.0f;
    cursorpos.s[2] = 0.0f;
    cursorpos.s[3] = 0.0f;
    scene.initScene();
    while (!glfwWindowShouldClose(env.window))
    {
	    scene.queryInput();
        scene.draw(shader);
	    if (scene.needInit)
            scene.initScene();
        else
            scene.animate(cursorpos);
        //scene.animate(cursorpos);
        env.updateFpsCounter();
        glfwSwapBuffers(env.window);
        glfwPollEvents();
	    camera.update();
        if (glfwGetKey (env.window, GLFW_KEY_SPACE)) {
            anim = true;
        }
        if (glfwGetKey (env.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(env.window, 1);
        }
    }
    glfwTerminate();
}
