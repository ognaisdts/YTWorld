#pragma once
class IScene
{
public:
	virtual void BeginScene() = 0;
	virtual void UpdateScene(float deltaTime) = 0;
	virtual void EndScene() = 0;
};

