#pragma once
#include "EditorWidget.h"

class CEditorSlider : public CEditorWidget
{
private:
	friend class CEditorWindow;
	friend class CEditorGroup;
	template <typename T>
	friend class CEditorTreeItem;
	std::string		m_Text;
	std::wstring	m_wText;
	std::string		m_TextUTF8;
	std::function<void()>	m_SliderCallback;
	float			m_Position[4];
	float			m_PositionMax;
	float			m_PositionMin;
protected:
	CEditorSlider();
	virtual ~CEditorSlider();
public:
	const std::string& GetText()	const
	{
		return m_Text;
	}
	const std::wstring& GetWText()	const
	{
		return m_wText;
	}
	const std::string& GetTextUTF8()	const
	{
		return m_TextUTF8;
	}
	const float GetPosition(int index)	const
	{
		return m_Position[index];
	}
	const float GetPositionMax()	const
	{
		return m_PositionMax;
	}
	const float GetPositionMin()	const
	{
		return m_PositionMin;
	}
	void SetText(const std::string& text);
	void SetPosition(int index,float f)
	{
		m_Position[index] = f;
	}
	void SetPositionMax(float f)
	{
		m_PositionMax = f;
	}
	void SetPositionMin(float f)
	{
		m_PositionMin = f;
	}
	void AddText(const std::string& text);
	virtual bool Init();
	virtual void Render();
	template <typename T>
	void SetSliderCallback(T* obj, void(T::* func)())
	{
		m_SliderCallback = std::bind(func, obj);
	}
};