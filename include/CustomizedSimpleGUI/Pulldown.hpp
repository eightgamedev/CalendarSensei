#pragma once

namespace CustomizedSimpleGUI
{

	class Pulldown
	{
	public:
		Pulldown(const Font font, const Vec2& displayPos, const double width, const size_t listCount);
		Pulldown(ListBoxState state, const Font font, const Vec2& displayPos, const double width, const size_t listCount);
		ListBoxState m_state;
		void emplace_back(String item);
		void setItems(const Array<String>& items);
		void setIndex(const size_t index);
		void setIndexFromItem(const String& item);
		size_t getSelectedIndex() const;
		size_t getItemsCount() const;
		String getSelectedItem() const;
		bool getIsOpen() const;
		void update();
		void draw() const;
		RectF getDisplayRegion() const;
		static void closeCurrentOpeningListBox();
		bool isChanged() const;
	private:
		static Pulldown* currentOpeningListBox;
		Font m_font;
		int32 m_fontHeight;
		Vec2 m_displayPos;
		RectF m_displayRegion;
		bool m_isOpen;
		size_t m_listCount;
		int32 m_maxLines;
		int32 m_lines;
		RectF m_listRegion;
		Optional<size_t> oldState;
		bool hasScrollBar;
		Vec2 m_listPos;
		int32 itemWidth;
	};

}
