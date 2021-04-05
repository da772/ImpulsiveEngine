#pragma once
#include <GEngine.h>

using namespace GEngine;

UCLASS()
class TestScript : public Component {

public:
	UCONSTRUCTOR()
	TestScript(Entity* e);

	~TestScript();

	UFUNCTION()
	virtual void UnloadGraphics() override;

	UFUNCTION()
	virtual void ReloadGraphics() override;
    
	UFUNCTION()
	virtual void OnBegin() override;

	UFUNCTION()
	virtual void OnEnd() override;

	UFUNCTION()
	virtual void OnUpdate(Timestep timestep) override;

	UFUNCTION()
	void TestFunc();

	UFUNCTION()
	void NewFunct(int i);

};


UCLASS()
class MyScript : public Component {
public:
	UCONSTRUCTOR()
	inline MyScript(Entity* e) : Component(e) {  };
	inline ~MyScript() {};

	UPROPERTY()
	int id = 123;

	UFUNCTION()
	inline int GetId() { return id;  }

	UFUNCTION()
	inline int* GetIdPtr() { return &id; }
};

