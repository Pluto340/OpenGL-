#include"GL/glew.h"
#include"GLFW/glfw3.h"
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include<iostream>
#include"texture.h"
#include"Camera.h"
#include"Shader.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_glfw_gl3.h"
#include"vector"
#include"map"
#include"DataStorage.h"
#include"framebuffer.h"
#include"Model.h"
using namespace std;

int WIDTH = 800;
int HEIGHT = 600;

unsigned int frameibo;
unsigned int textureAttachment;
unsigned int rendibo;
unsigned int amount = 100;
glm::mat4* modelMatrices;
Camera camera;

int ExamplesMillion(Model& rockmodel);
unsigned int locadCubeMap(vector<string>& faces);
void rotatemodel(unsigned int amount,unsigned int rockbuffer, Model& rockmodel);

int main()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		cout << "glfw init error" << endl;
		return -1;
	}
	window = glfwCreateWindow(WIDTH, HEIGHT, "review opengl", NULL,NULL);

	glfwSetFramebufferSizeCallback(window,ProcessInput);
	glfwSetCursorPosCallback(window,mouse_callback);
	glfwSetScrollCallback(window,scroll_callback);

	if (!window)
	{
		cout << "create window error" << endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		cout << "glew init error" << endl;
		return -1;
	}
	//启用深度测试和模板测试
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);

	//一开始模板缓冲的默认值是0，所以我们在不等于1的位置绘制物体。
	glStencilFunc(GL_NOTEQUAL,1,0xFF);

	////如果通过测试，就将模板缓冲中的值设置为指定的ref值。
                                              //如果禁用了模板更新，此语句作废。
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);//都通过时，将模板值设置为ref值
	Shader objectshader("objector.editorconfig");
	Shader Stencilshader("Stencil.editorconfig");
	Shader transparentshader("transparent.editorconfig");
	Shader oppsitetextureshader("Opposite_Color_Texture.editorconfig");
	Shader Rendershader("Render.editorconfig");
	Shader Greyscaleshader("greyscale.editorconfig");
	Shader NuclearRuiHuashader("nuclear_effects.editorconfig");
	Shader EdgeDetectionshader("EdgeDetection.editorconfig");
	Shader NuclearBlurringshader("Nuclear_Blurring.editorconfig");
    Shader skyboxshader("skybox.editorconfig");
	Shader EnvironmentReflectShader("envirnoment_reflect.editorconfig");
	Shader EnvironmentRefractShader("envirnoment_refract.editorconfig");
	Shader Explodeshader("Explode.editorconfig");
	Shader Explodemodelshader("model.editorconfig");
	Shader Examplesshader("MultiExamples.editorconfig");
	Shader NormalModelshader("NormalModel.editorconfig");

	//每根草位置
	vector<glm::vec3>grassPositions{
		glm::vec3(-1.f, -1.0f, -1.0f),
		glm::vec3(3.5f, -1.009f, 0.6f),
		glm::vec3(0.5f, -1.0f, 0.51f),
		glm::vec3(-0.3f, -1.0f, -2.3f),
		glm::vec3(0.5f, -1.0f, -0.6f)
	};
	//每个窗户位置
	vector<glm::vec3>windowPositions{
		glm::vec3(-0.5f, -1.0f, 0.51f),
		glm::vec3(2.0f,-1.0f,4.0f),
		glm::vec3(2.5f,-1.0f,3.2f),
		glm::vec3(0.51f,1.0f,2.91f)
	}; 
	//立方体贴图位置
	vector<string> faces{
		"skybox/right.jpg",
		"skybox/left.jpg",
		"skybox/top.jpg",
		"skybox/bottom.jpg",
		"skybox/front.jpg",
		"skybox/back.jpg"
	};
	unsigned int skyboxTexture=locadCubeMap(faces);

	//草缓冲区
	unsigned int transparentVAO, transparentVBO;
	TransparentDataStorage(transparentVAO,transparentVBO);

	unsigned int floorVAO, floorVBO;
	FloorDataStorage(floorVAO,floorVBO);

	unsigned int CubeVAO, CubeVBO;
	CubeDataStorage(CubeVAO,CubeVBO);

	unsigned int QuadVAO, QuadVBO;
	QuadDataStorage(QuadVAO,QuadVBO);

	unsigned int skyboxVAO, skyboxVBO;
	SkyboxDataStorage(skyboxVAO,skyboxVBO);

	unsigned int ExitNormalCubeVAO, ExitNormalCubeVBO;
	ExitNormalCubeDataStorage(ExitNormalCubeVAO,ExitNormalCubeVBO);

	unsigned int ExplodeCubeVAO, ExplodeCubeVBO;
	ExPlodeCubeDataStorage(ExplodeCubeVAO,ExplodeCubeVBO);

	Texture floortexture1("textures/floor.jpg");
	Texture floortexture2("textures/flower.png");
	Texture Cubetexture1("textures/Cubebox.jpg");
	Texture Cubetexture2("textures/Cubecolor.jpg");
	Texture Grasstexture("textures/grass.png");
	Texture Windowtexture("textures/window.png");
	Texture Woodboxtexture("textures/container.jpg");

	unsigned int objectBlock = glGetUniformBlockIndex(objectshader.GetId(), "ExampleBlock");
	unsigned int stencilBlock = glGetUniformBlockIndex(Stencilshader.GetId(), "ExampleBlock");
	unsigned int RelectBlock = glGetUniformBlockIndex(EnvironmentReflectShader.GetId(),"ExampleBlock");
	unsigned int RefractBlock = glGetUniformBlockIndex(EnvironmentRefractShader.GetId(),"ExampleBlock");
	unsigned int EdgeDetectionBlock = glGetUniformBlockIndex(EdgeDetectionshader.GetId(),"ExampleBlock");
	unsigned int BlurBlock = glGetUniformBlockIndex(NuclearBlurringshader.GetId(),"ExampleBlock");
	unsigned int RuiHuaBlock = glGetUniformBlockIndex(NuclearRuiHuashader.GetId(),"ExampleBlock");
	unsigned int GreyscaleBlock = glGetUniformBlockIndex(Greyscaleshader.GetId(),"ExampleBlock");
	unsigned int TransparentBlock = glGetUniformBlockIndex(transparentshader.GetId(),"ExampleBlock");
	unsigned int OppositeBlock = glGetUniformBlockIndex(oppsitetextureshader.GetId(),"ExampleBlock");
	unsigned int ExplodeCubeBlock = glGetUniformBlockIndex(Explodeshader.GetId(),"ExampleBlock");
	unsigned int ExplodeModelBlock = glGetUniformBlockIndex(Explodemodelshader.GetId(),"ExampleBlock");
	unsigned int ExamplesBlock = glGetUniformBlockIndex(Examplesshader.GetId(),"ExampleBlock");
	unsigned int NormalmodelBlock= glGetUniformBlockIndex(NormalModelshader.GetId(), "ExampleBlock");

	glUniformBlockBinding(objectshader.GetId(), objectBlock, 0);
	glUniformBlockBinding(Stencilshader.GetId(), stencilBlock, 0);
	glUniformBlockBinding(EnvironmentReflectShader.GetId(), RelectBlock, 0);
	glUniformBlockBinding(EnvironmentRefractShader.GetId(), RefractBlock, 0);
	glUniformBlockBinding(EdgeDetectionshader.GetId(), EdgeDetectionBlock, 0);
	glUniformBlockBinding(NuclearBlurringshader.GetId(), BlurBlock, 0);
	glUniformBlockBinding(NuclearRuiHuashader.GetId(), RuiHuaBlock, 0);
	glUniformBlockBinding(transparentshader.GetId(), TransparentBlock, 0);
	glUniformBlockBinding(oppsitetextureshader.GetId(), OppositeBlock, 0);
	glUniformBlockBinding(Greyscaleshader.GetId(), GreyscaleBlock, 0);
	glUniformBlockBinding(Explodeshader.GetId(),ExplodeCubeBlock,0);
	glUniformBlockBinding(Explodemodelshader.GetId(),ExplodeModelBlock,0);
	glUniformBlockBinding(Examplesshader.GetId(), ExamplesBlock, 0);
	glUniformBlockBinding(NormalModelshader.GetId(), NormalmodelBlock, 0);

	unsigned int MatriciesBlock;
	glGenBuffers(1,&MatriciesBlock);
	//预留内存
	glBindBuffer(GL_UNIFORM_BUFFER,MatriciesBlock);
	glBufferData(GL_UNIFORM_BUFFER,2*sizeof(glm::mat4),NULL,GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);

	glBindBufferRange(GL_UNIFORM_BUFFER,0,MatriciesBlock,0,2*sizeof(glm::mat4));


	objectshader.Bind();
	objectshader.setInt("texture1",0);
	objectshader.setInt("texture2", 1);

	transparentshader.Bind();
	transparentshader.setInt("texture1",0);

	oppsitetextureshader.Bind();
	oppsitetextureshader.setInt("screenTexture",0);

	Rendershader.Bind();
	Rendershader.setInt("screenTexture",0);

	Greyscaleshader.Bind();
	Greyscaleshader.setInt("screenTexture",0);

	NuclearRuiHuashader.Bind();
	NuclearRuiHuashader.setInt("screenTexture",0);

	EdgeDetectionshader.Bind();
	EdgeDetectionshader.setInt("screenTexture",0);

	NuclearBlurringshader.Bind();
	NuclearBlurringshader.setInt("screenTexture",0);

	skyboxshader.Bind();
	skyboxshader.setInt("skybox",0);

	EnvironmentReflectShader.Bind();
	EnvironmentReflectShader.setInt("skybox",0);

	EnvironmentRefractShader.Bind();
	EnvironmentRefractShader.setInt("skybox", 0);

	Model ourmodel("nanosuit/nanosuit.obj");
	Model rockmodel("79076/79076.obj");
	Model Stedevkmodel("New_Stedev/Steve.obj");
    Model Stantamodel("stanta/Santa.obj");
    Model Ballonmodel("ballon/11809_Hot_air_balloon_l2.obj");
    Model Chrismatree("Christmas_tree/79181.obj");

	SetFramebuffer();//帧缓冲
	while (!glfwWindowShouldClose(window))
	{
		PressKey(window);

		glBindFramebuffer(GL_FRAMEBUFFER, frameibo);
		glClearColor(0.6f,0.3f,0.5f,1.0f);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
		

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(camera.CameraPos,camera.CameraFront+camera.CameraPos,camera.CameraUp);
		glm::mat4 projection;
		if (WIDTH != 0 || HEIGHT != 0)
			projection = glm::perspective(glm::radians(camera.m_fov), (float)WIDTH / HEIGHT, 0.1f, 1000.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, MatriciesBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBuffer(GL_UNIFORM_BUFFER, MatriciesBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glStencilMask(0x00);//关闭模板缓冲区的写入
		objectshader.Bind();
		glBindVertexArray(CubeVAO);
		model = glm::translate(model,glm::vec3(0.0f,-1.0f,0.0f));
		Cubetexture1.Bind(0);
		Cubetexture2.Bind(1);
		objectshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(floorVAO);
		objectshader.Bind();
		floortexture1.Bind(0);
		floortexture2.Bind(1);
		objectshader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		
		glBindVertexArray(transparentVAO);
		transparentshader.Bind();
		Grasstexture.Bind(0);
		for (unsigned int i = 0; i < grassPositions.size(); i++)
		{
			model = glm::mat4(1.0f);
			transparentshader.setMat4("model",glm::translate(model,grassPositions[i]));
			glDrawArrays(GL_TRIANGLES,0,6);
		}

		glBindVertexArray(0);

		//面剔除
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);
		glBindVertexArray(CubeVAO);
		objectshader.Bind();
		Cubetexture1.Bind(0);
		floortexture2.Bind(1);
		model = 1.0f;
		objectshader.setMat4("model",glm::translate(model,glm::vec3(5.3f,-0.9f,7.0f)));
		glDrawArrays(GL_TRIANGLES,0,36);
		glDisable(GL_CULL_FACE);

		//所有片段都更新模板缓冲，所有的片段都应该更新模板缓冲，通过使用GL_ALWAYS模板测试函数，我们保证了箱子的每个片段都会将模板缓冲的模板值更新为1。因为片段永远会通过模板测试，在绘制片段的地方，模板缓冲会被更新为参考值。
		glStencilFunc(GL_ALWAYS,1,0xFF);
		glStencilMask(0xFF);//启用模板缓冲

		glBindVertexArray(CubeVAO);
		objectshader.Bind();
		Cubetexture1.Bind(0);
		Cubetexture2.Bind(1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -0.99999f, 0.0f));
		objectshader.setMat4("model",model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		glStencilFunc(GL_NOTEQUAL,1,0xFF);//在模板不为1的地方绘制边框
		glStencilMask(0x00);////禁用模板写入，因为模板缓冲在箱子被绘制的地方都更新为1了，绘制放大的箱子
		//这里删掉了，别加上///glDisable(GL_DEPTH_TEST);//禁用深度测试，防止边框被地面遮挡

		float scale = 1.1f;
		Stencilshader.Bind();
		model = glm::scale(model,glm::vec3(scale,scale,scale));
		Stencilshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
		//为下一次模板缓冲清零和渲染做准备；
		glStencilMask(0xFF);//启用模板测试
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
		

		glBindVertexArray(transparentVAO);
		transparentshader.Bind();
		Windowtexture.Bind(0);
		map<float, glm::vec3>rendwindow;
		for (unsigned int i = 0; i < windowPositions.size(); i++)
		{
			float length = glm::length(camera.CameraPos - windowPositions[i]);
			rendwindow[length] = windowPositions[i];
		}
		for (map<float, glm::vec3>::reverse_iterator it = rendwindow.rbegin(); it != rendwindow.rend(); it++)
		{
			model = glm::mat4(1.0f);
			transparentshader.setMat4("model", glm::translate(model, it->second));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindVertexArray(0);

        //灰度
        glBindVertexArray(CubeVAO);
        Greyscaleshader.Bind();
        Woodboxtexture.Bind(0);
        model = glm::mat4(1.0f);
		Greyscaleshader.setMat4("model",glm::translate(model,glm::vec3(3.0f,3.0f,4.0f)));
		glDrawArrays(GL_TRIANGLES,0,36);

		//核效果.锐化
		glBindVertexArray(CubeVAO);
		NuclearRuiHuashader.Bind();
		Woodboxtexture.Bind(0);
		model = glm::mat4(1.0f);
		NuclearRuiHuashader.setMat4("model", glm::translate(model,glm::vec3(1.0f,1.0f,2.4f)));
		glDrawArrays(GL_TRIANGLES,0,36);

        //反相
		glBindVertexArray(CubeVAO);
		oppsitetextureshader.Bind();
		Woodboxtexture.Bind(0);
		oppsitetextureshader.setMat4("projection", projection);
		oppsitetextureshader.setMat4("view", view);
		model = glm::mat4(1.0f);
		oppsitetextureshader.setMat4("model", glm::translate(model, glm::vec3(7.0f, 3.0f, 1.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//核效果.模糊
		glBindVertexArray(CubeVAO);
		NuclearBlurringshader.Bind();
		Woodboxtexture.Bind(0);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime()+90.0f, glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(4.0f, 3.0f, 1.0f));
		//先rotate再translate能让他围绕一个原点转
		//先translate再rotate能让它原地旋转
		NuclearBlurringshader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//核效果.边缘检测
		glBindVertexArray(CubeVAO);
		EdgeDetectionshader.Bind();
		Woodboxtexture.Bind(0);
		model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)sin(glm::radians(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(1.0f, -0.9999f, 1.0f));
		EdgeDetectionshader.setMat4("model", model );
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//环境反射
		glBindVertexArray(ExitNormalCubeVAO);
		EnvironmentReflectShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(11.0f, 3.0f, -9.0f));
		model = glm::rotate(model,(float)glfwGetTime(),glm::vec3(0.0f,1.0f,0.0));
		EnvironmentReflectShader.setMat4("model",glm::translate(model, glm::vec3(11.0f, -.0f, -9.0f)));
		EnvironmentReflectShader.setVec3("cameraPos",camera.CameraPos);
		glDrawArrays(GL_TRIANGLES,0,36);

		//环境折射
		glBindVertexArray(ExitNormalCubeVAO);
		EnvironmentRefractShader.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, 3.0f, -9.0f));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0));
		EnvironmentRefractShader.setMat4("model", glm::translate(model, glm::vec3(11.0f, -.0f, -9.0f)));
		EnvironmentRefractShader.setVec3("cameraPos", camera.CameraPos);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//爆炸效果
		glBindVertexArray(ExplodeCubeVAO);
		Explodeshader.Bind();
		Cubetexture1.Bind(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model,glm::vec3(7.0f,4.0f,9.0f));
		model = glm::rotate(model,glm::radians(45.0f),glm::vec3(0.0f,0.0f,1.0f));
		Explodeshader.setMat4("model",model);
		Explodeshader.setFloat("time", glfwGetTime());
		glDrawArrays(GL_TRIANGLES,0,36);
		glBindVertexArray(0);
		
		//先知模型
		Explodemodelshader.Bind();
		Explodemodelshader.setMat4("view", view);
		Explodemodelshader.setMat4("projection",projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model,glm::vec3(-9.0f,-0.5f,3.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		Explodemodelshader.setMat4("model",model);
		Explodemodelshader.setFloat("time",glfwGetTime());
		ourmodel.Draw(Explodemodelshader);


		//鲸鱼模型
	/*	Examplesshader.Bind();
		Examplesshader.setInt("diffuse1",0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rockmodel.textures_load[0].id);
        ExamplesMillion(rockmodel);
		for (unsigned int i = 0; i < rockmodel.meshes.size(); i++)
		{
			glBindVertexArray(rockmodel.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES,static_cast<unsigned int>(rockmodel.meshes[i].indices.size()),GL_UNSIGNED_INT,0,amount);
			glBindVertexArray(0);
		}*/

		//史蒂夫
		NormalModelshader.Bind();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, -1.5f, 5.0f));
		model = glm::scale(model,glm::vec3(0.5,0.5,0.5));
		model = glm::rotate(model,(float)glfwGetTime(),glm::vec3(0.0,1.0,0.0));
		NormalModelshader.setMat4("model",model);
		Stedevkmodel.Draw(NormalModelshader);

        //圣诞老人
        NormalModelshader.Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, -1.5f, -1.0f));
        model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
        model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        NormalModelshader.setMat4("model", model);
        Stantamodel.Draw(NormalModelshader);

        //热气球
        NormalModelshader.Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-19.0f, -1.5f, 11.0f));
        model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
        model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        NormalModelshader.setMat4("model", model);
        Ballonmodel.Draw(NormalModelshader);

        //圣诞树
        NormalModelshader.Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, -1.5f, 7.0f));
        model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
        model = glm::rotate(model, (float)glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        NormalModelshader.setMat4("model", model);
        Chrismatree.Draw(NormalModelshader);

		//天空盒
		glDepthFunc(GL_LEQUAL);
		glBindVertexArray(skyboxVAO);
		skyboxshader.Bind();
		skyboxshader.setMat4("projection",projection);
		skyboxshader.setMat4("view",glm::mat4(glm::mat3(view)));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture);
		glDrawArrays(GL_TRIANGLES,0,36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);//disable depth test so screen-space quad isn't discarded due to depth test.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT );

		glBindVertexArray(QuadVAO);
		Rendershader.Bind();
		glBindTexture(GL_TEXTURE_2D,textureAttachment);
		glDrawArrays(GL_TRIANGLES,0,6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//glDeleteTextures(1,&skyboxTexture);
	glfwTerminate();
	return 0;
}

unsigned int locadCubeMap(vector<string>& faces)
{
	unsigned int skyboxTexture;
	glGenTextures(1,&skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP,skyboxTexture);
	
	int height, width, BPP, i = 0;
	for (auto it = faces.begin(); it != faces.end(); it++)
	{
		
		stbi_set_flip_vertically_on_load(false);
		unsigned char* data = stbi_load(it->c_str(),&width,&height,&BPP,4);

		if (data)
		{
			cout << "天空盒纹理加载成功！" << endl;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			stbi_image_free(data);
		}
		else
			cout << "skyboxCube error： " << *it << "  " << stbi_failure_reason() << endl;
		cout << i << endl;
		i++;
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return skyboxTexture;
}
int ExamplesMillion(Model &rockmodel)
{
	
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = (rand() % 20) / 10.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand() % 360);
		model = glm::rotate(model, (float)(glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::translate(model, glm::vec3(x, y, z));
		modelMatrices[i] = model;
	}

	unsigned int rockbuffer;
	glGenBuffers(1, &rockbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, rockbuffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rockmodel.meshes.size(); i++)
	{
		unsigned int VAO = rockmodel.meshes[i].VAO;
		glBindVertexArray(VAO);
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
	return rockbuffer;
}
void rotatemodel(unsigned int amount,unsigned int rockbuffer,Model &rockmodel)
{
	srand(glfwGetTime());
	float radius = 150.0;
	float offset = 25.0f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		float scale = (rand() % 20) / 10.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		float rotAngle = (rand() % 360);
		model = glm::rotate(model, (float)(glfwGetTime()), glm::vec3(0.0f, 0.1f, 0.0f));
		model = glm::translate(model, glm::vec3(x, y, z));

		modelMatrices[i] = model;
	}
	glBindBuffer(GL_ARRAY_BUFFER, rockbuffer);
    for (unsigned int i = 0; i < rockmodel.meshes.size(); i++)
    {
        unsigned int VAO = rockmodel.meshes[i].VAO;
        glBindVertexArray(VAO);
        GLsizei vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}