#pragma once
class InputManager;
class TitleScene
{
private:
	bool m_isStartRequested;
public:
	TitleScene();
	~TitleScene();

	void Initialize(InputManager&inputmanager);
	void Update(InputManager&inputmanager);
	void Render();
	void Finalize();

	bool IsStartRequested()const;
};

