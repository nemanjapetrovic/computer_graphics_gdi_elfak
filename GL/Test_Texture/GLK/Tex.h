#pragma once
class CTex
{
public:
	UINT id;
	CTex();
	~CTex();
	void Release();
	void Select();
	void LoadTexture(CString fileName);
	void PrepareTexture(bool lightEnabled);
};

