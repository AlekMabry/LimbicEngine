#pragma once

class LObject
{
public:
	LObject();

	virtual void OnTick(float dt);

	void SetTickEnabled(bool bIsTickEnabled);

	bool IsTickEnabled();

private:
	bool bIsTickEnabled;
};
