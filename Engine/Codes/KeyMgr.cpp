#include "KeyMgr.h"

_USING(Engine)
_IMPLEMENT_SINGLETON(KeyMgr)

KeyMgr::KeyMgr()
{
	ZeroMemory(m_bKeyDown, KEY_MAX * sizeof(bool));
	ZeroMemory(m_bKeyUp, KEY_MAX * sizeof(bool));

	ZeroMemory(m_bMouseDown, KEY_MAX * sizeof(bool));
	ZeroMemory(m_bMouseUp, KEY_MAX * sizeof(bool));
}

bool KeyMgr::KeyDown(int key)
{
	if (m_keyState[key] & 0x80)
	{
		if (!m_bKeyDown[key])
		{
			m_bKeyDown[key] = true;
			return true;
		}
	}
	else
	{
		m_bKeyDown[key] = false;
	}

	return false;
}

bool KeyMgr::KeyUp(int key)
{
	if (m_keyState[key] & 0x80)
	{
		m_bKeyUp[key] = true;
	}
	else
	{
		if (m_bKeyUp[key])
		{
			m_bKeyUp[key] = false;
			return true;
		}
	}

	return false;
}

bool KeyMgr::KeyPressing(int key)
{
	if (m_keyState[key] & 0x80)
		return true;
	else
		return false;
}

bool KeyMgr::MouseButtonDown(int buttonNum)
{
	if (m_mouseState.rgbButtons[buttonNum] & 0x80)
	{
		if (!m_bMouseDown[buttonNum])
		{
			m_bMouseDown[buttonNum] = true;
			return true;
		}
	}
	else
	{
		m_bMouseDown[buttonNum] = false;
	}

	return false;
}

bool KeyMgr::MouseButtonUp(int buttonNum)
{
	if (m_mouseState.rgbButtons[buttonNum] & 0x80)
	{
		m_bMouseUp[buttonNum] = true;
	}
	else
	{
		if (m_bMouseUp[buttonNum])
		{
			m_bMouseUp[buttonNum] = false;
			return true;
		}
	}

	return false;
}

bool KeyMgr::MouseButtonPressing(int buttonNum)
{
	if (m_mouseState.rgbButtons[buttonNum] & 0x80)
		return true;
	else
		return false;
}

long KeyMgr::MouseMove(int dir)
{
	return *((long*)&m_mouseState + dir);
}

bool KeyMgr::MouseWheelDown()
{
	if (m_mouseState.lZ < 0)
		return true;

	return false;
}

bool KeyMgr::MouseWheelUp()
{
	if (m_mouseState.lZ > 0)
		return true;

	return false;
}

HRESULT KeyMgr::Init(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(ReadyKeyBoard(hWnd)))
		return E_FAIL;

	if (FAILED(ReadyMouse(hWnd)))
		return E_FAIL;


	return NOERROR;
}

void KeyMgr::UpdateKeyMgr()
{
	if (nullptr == m_pKeyBoard || nullptr == m_pMouse)
		return;

	if (FAILED(m_pKeyBoard->GetDeviceState(KEY_MAX, m_keyState)))
		m_pKeyBoard->Acquire();

	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState)))
		m_pMouse->Acquire();
}

HRESULT KeyMgr::ReadyKeyBoard(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard); // ǥ��Ű���忡 ���� ������ ��.

	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // Ű������ ���������� �����Ѵ�.

	m_pKeyBoard->Acquire(); // ���� Ű���尡 ������ �� �ֵ��� Ȱ��ȭ�Ѵ�.

	return NOERROR;
}

HRESULT KeyMgr::ReadyMouse(HWND hWnd)
{
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	m_pMouse->SetDataFormat(&c_dfDIMouse); // ǥ��Ű���忡 ���� ������ ��.

	m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY); // Ű������ ���������� �����Ѵ�.

	m_pMouse->Acquire(); // ���� Ű���尡 ������ �� �ֵ��� Ȱ��ȭ�Ѵ�.

	return NOERROR;
}

DWORD KeyMgr::Free()
{
	m_pKeyBoard->Release();
	m_pMouse->Release();
	m_pInputSDK->Release();

	return 0;
}
