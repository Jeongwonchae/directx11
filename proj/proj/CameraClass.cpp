#include "Stdafx.h"
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

	//������ ����Ű�� ���� ����
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//XMVECTOR ����ü�� ���ε�
	upVector = XMLoadFloat3(&up);

	//3D���忡�� ī�޶��� ��ġ�� ����
	position = m_position;

	//XMVECTOR ����ü�� �ε�
	positionVector = XMLoadFloat3(&position);

	//�⺻������ ī�޶� ã���ִ� ��ġ�� ����
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	//XMVECTOR ����ü�� �ε�
	lookAtVector = XMLoadFloat3(&lookAt);

	float toRadian = 0.174532925f;
	// yaw(y��), pitch(x��) �� roll(z��)�� ȸ������ ���� ������ ����
	pitch = m_rotation.x * toRadian;
	yaw = m_rotation.y * toRadian;
	roll = m_rotation.z * toRadian;

	//yaw, pitch, roll ���� ���� ȸ�� ����� ����
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//lookAt �� up ���͸� ȸ�� ��ķ� �����Ͽ� �䰡 �������� �ùٸ��� ȸ���ǵ��� ��
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	//ȸ�� �� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//���������� �� ���� ������Ʈ �� ���Ϳ��� �� ����� ����
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
