#include "Transform.h"

using namespace DirectX;

C_Transform::C_Transform() {
	position = XMFLOAT3(0,0,0);
	scale = XMFLOAT3(1,1,1);
	rotation = XMFLOAT3(0,0,0);
	quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
}

C_Transform::~C_Transform() {

}

void C_Transform::RotateAround(XMFLOAT3 center, XMFLOAT3 axis, float deg) {

	XMFLOAT3 diff;

	XMFLOAT3 rotateCenter(position.x - center.x, position.y - center.y, position.z - center.z);

	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&rotateCenter), XMMatrixRotationQuaternion(XMQuaternionRotationAxis(XMLoadFloat3(&axis), XMConvertToRadians(deg))));

	XMStoreFloat3(&diff, vec);

	position.x = center.x + diff.x;
	position.y = center.y + diff.y;
	position.z = center.z + diff.z;
}

void C_Transform::Rotate(XMFLOAT3 rot) {
	rotation.x += XMConvertToRadians(rot.x);
	rotation.y += XMConvertToRadians(rot.y);
	rotation.z += XMConvertToRadians(rot.z);
	quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
}

XMFLOAT3 PositionLerp(XMFLOAT3 toward, XMFLOAT3 fromward, float per) {
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMVectorLerp(XMLoadFloat3(&toward), XMLoadFloat3(&fromward), per));
	return temp;
}

XMVECTOR RotationLerp(XMVECTOR toward, XMVECTOR fromward, float per) {
	return XMQuaternionSlerp(toward, fromward, per);
}

void C_Transform::setRotation(XMFLOAT3 rot) {
	rotation.x = XMConvertToRadians(rot.x);
	rotation.y = XMConvertToRadians(rot.y);
	rotation.z = XMConvertToRadians(rot.z);

	quaternion = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
}

void C_Transform::setPosition(XMFLOAT3 pos) {
	position = pos;
}

void C_Transform::setScale(XMFLOAT3 scl) {
	scale = scl;
}

XMFLOAT4X4 C_Transform::getWorldMatrix() {
	XMFLOAT4X4 out;
	XMMATRIX mat = XMMatrixTransformation(XMLoadFloat3(&zero), XMLoadFloat3(&zero), XMLoadFloat3(&scale), XMLoadFloat3(&zero), quaternion, XMLoadFloat3(&position));
	XMStoreFloat4x4(&out, mat);
	return out;
}


XMFLOAT3 C_Transform::getPosition() {
	return position;
}
XMFLOAT3 C_Transform::getRotation() {
	XMFLOAT3 rot;
	rot.x = XMConvertToDegrees(rotation.x);
	rot.y = XMConvertToDegrees(rotation.y);
	rot.z = XMConvertToDegrees(rotation.z);
	return rot;
}

XMFLOAT4 C_Transform::getQuaternion() {
	XMFLOAT4 qt;
	XMStoreFloat4(&qt, quaternion);
	return qt;
}

XMFLOAT3 C_Transform::getScale() {
	return scale;
}

XMFLOAT3 C_Transform::getForward() {
	XMFLOAT3 out = forward;

	XMMATRIX mtx = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&out),mtx);

	vec = XMVector3Normalize(vec);

	XMStoreFloat3(&out, vec);
	return out;
}

XMFLOAT3 C_Transform::getUp() {
	XMFLOAT3 out = up;

	XMMATRIX mtx = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&out), mtx);
	vec = XMVector3Normalize(vec);

	XMStoreFloat3(&out, vec);
	return out;
}

XMFLOAT3 C_Transform::getRight() {
	XMFLOAT3 out = right;

	XMMATRIX mtx = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMVECTOR vec = XMVector3TransformCoord(XMLoadFloat3(&out), mtx);
	vec = XMVector3Normalize(vec);

	XMStoreFloat3(&out, vec);
	return out;
}

void C_Transform::move(XMFLOAT3 move) {
	position.x += move.x;
	position.y += move.y;
	position.z += move.z;
}

void C_Transform::setQuaternion(XMVECTOR vec) {
	quaternion = vec;
}