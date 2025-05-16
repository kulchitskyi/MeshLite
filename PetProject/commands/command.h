#pragma once
#include "Icommand.h" 

#include <memory>

class Scene;
class SimplificationStrategy;

class SceneCommand
{
protected:
	std::shared_ptr<Scene> _scene;
public:
	void setScene(std::shared_ptr<Scene> scene);
	auto getScene() -> std::shared_ptr<Scene>;
	virtual void execute() = 0;
	virtual ~SceneCommand() = default;
};

class CopyCommand : public SceneCommand
{
public:
	void execute() override;
};