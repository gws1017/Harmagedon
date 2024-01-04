#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

//����� ���漱�� �� ��

UCLASS()
class PROJECTH_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

//ACharacter Ŭ���� �⺻ ��� �Լ�
//Ŭ���� ���� �� ��ӵǴ� �⺻�Լ� ������� �ʴ´ٸ� �����ֱ�
protected:
	
	//virtual void BeginPlay() override;

public:	
	
	//virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	//�ܺο��� ȣ��Ǵ� �Լ� �ۼ�

	//�ܺο��� ������ �� �ִ� ���� �ۼ�(�ǵ����̸� ������ private�� �ۼ��ϰ� Getter Setter �̿��� ��)

private:

	//ĳ���� ���ο����� ȣ��Ǵ� �Լ� �ۼ� (�ַ� Ű�Է�)

	//Ű�Է� ���� �Լ�
	void OnMoveForward(float Axis);
	void OnMoveRight(float Axis);
	void OnHorizonLock(float Axis);
	void OnVerticalLock(float Axis);

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, Category = "Component")
		class UCameraComponent* Camera;
};
