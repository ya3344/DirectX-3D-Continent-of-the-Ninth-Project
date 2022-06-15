#include "InputDevice.h"


_USING(Engine)
_IMPLEMENT_SINGLETON(InputDevice)

InputDevice::InputDevice()
{
}



HRESULT InputDevice::Init(HINSTANCE hInst, HWND hWnd)
{
	// m_pInput_SDK �� �����Ѵ�.
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(ReadyKeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(ReadyMouse(hWnd)))
		return E_FAIL;

	return NOERROR;
}

void InputDevice::SetUp_DeviceState()
{
	if (nullptr == m_pKeyBoard || nullptr == m_pMouse)
		return;

	m_pKeyBoard->GetDeviceState(256, m_KeyState);

	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);

}

HRESULT InputDevice::ReadyKeyBoard(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard); // ǥ��Ű���忡 ���� ������ ��.

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // Ű������ ���������� �����Ѵ�.

	m_pKeyBoard->Acquire(); // ���� Ű���尡 ������ �� �ֵ��� Ȱ��ȭ�Ѵ�.

	return NOERROR;
}

HRESULT InputDevice::ReadyMouse(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse); // ǥ��Ű���忡 ���� ������ ��.

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // Ű������ ���������� �����Ѵ�.

	m_pMouse->Acquire(); // ���� Ű���尡 ������ �� �ֵ��� Ȱ��ȭ�Ѵ�.

	return NOERROR;
}

bool InputDevice::MouseLButton()
{
	return (mMouseState.rgbButtons[0] & 0x80) ? true : false;
}

bool InputDevice::MouseRButton()
{
	return (mMouseState.rgbButtons[1] & 0x80) ? true : false;
}

DWORD InputDevice::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);

	return DWORD();
}


