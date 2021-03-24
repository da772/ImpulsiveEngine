
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
    void NewReloadedFunction();
    
	UFUNCTION()
	virtual void OnBegin() override;

	UFUNCTION()
	virtual void OnEnd() override;

	UFUNCTION()
	virtual void OnUpdate(Timestep timestep) override;

	UFUNCTION()
	void TestFunc();

};

