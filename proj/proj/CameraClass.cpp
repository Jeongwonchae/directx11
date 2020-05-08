﻿#include "Stdafx.h"
#include "CameraClass.h"


CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass &)
{
}


CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	//위쪽을 가리키는 백터 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//XMVECTOR 구조체에 업로드
	upVector = XMLoadFloat3(&up);

	//3D월드에서 카메라의 위치를 설정
	position = m_position;

	//XMVECTOR 구조체에 로드
	positionVector = XMLoadFloat3(&position);

	//기본적으로 카메라가 찾고있는 위치를 설정
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//XMVECTOR 구조체에 로드
	lookAtVector = XMLoadFloat3(&lookAt);

	float toRadian = 0.174532925f;
	// yaw(y축), pitch(x축) 및 roll(z축)의 회전값을 라디안 단위로 설정
	pitch = m_rotation.x * toRadian;
	yaw = m_rotation.y * toRadian;
	roll = m_rotation.z * toRadian;

	//yaw, pitch, roll 값을 통해 회전 행렬을 만듬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//lookAt 및 up 벡터를 회전 행렬로 변형하여 뷰가 원점에서 올바르게 회전되도록 함
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//회전 된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 만듬
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void CameraClass::RenderReflection(float height)
{
	XMFLOAT3 up, position, lookAt;

	//업벡터 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//XMVECTOR 구조체에 로드
	XMVECTOR upVector = XMLoadFloat3(&up);

	//3D월드에서 카메라의 위치를 설정
	position.x = m_position.x;
	position.y = -m_position.y + (height*2.0f);
	position.z = m_position.z;

	//XMVECTOR 구조체에 로드
	XMVECTOR positionVector = XMLoadFloat3(&position);

	//Calcuulate the rotation in radians
	float radians = m_rotation.y * 0.174532925f;

	//기본적으로 카메라가 찾고있는 위치를 설정
	lookAt.x = sinf(radians) + m_position.x;
	lookAt.y = position.y;
	lookAt.z = cosf(radians) + m_position.z;

	//XMVECTOR 구도체에 로드
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);

	//마지막으로 세 개의 업데이트 된 벡터에서 뷰 행렬을 마듬
	m_reflectionViewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

XMMATRIX CameraClass::GetReflectionViewMatrix()
{
	return m_reflectionViewMatrix;
}
