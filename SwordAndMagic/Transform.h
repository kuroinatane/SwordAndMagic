#pragma once
#include "main.h"

constexpr DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
constexpr DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
constexpr DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
constexpr DirectX::XMFLOAT3 zero = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

class C_Transform {
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMVECTOR quaternion;
public:
	C_Transform();
	~C_Transform();

	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	DirectX::XMFLOAT3 getScale();
	DirectX::XMFLOAT4 getQuaternion();
	void setQuaternion(DirectX::XMVECTOR vec);

	void move(DirectX::XMFLOAT3);
	void setPosition(DirectX::XMFLOAT3);
	void setRotation(DirectX::XMFLOAT3);
	void setScale(DirectX::XMFLOAT3);

	void Rotate(DirectX::XMFLOAT3);

	void RotateAround(DirectX::XMFLOAT3, DirectX::XMFLOAT3, float);
	DirectX::XMFLOAT3 getForward();
	DirectX::XMFLOAT3 getUp();
	DirectX::XMFLOAT3 getRight();


	DirectX::XMFLOAT4X4 getWorldMatrix();
};

DirectX::XMFLOAT3 PositionLerp(DirectX::XMFLOAT3, DirectX::XMFLOAT3, float);
DirectX::XMVECTOR RotationLerp(DirectX::XMVECTOR, DirectX::XMVECTOR, float);