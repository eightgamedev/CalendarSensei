# include "CustomizedSimpleGUI/Pulldown.hpp"
# include "CustomizedSimpleGUI/GUIVariables.hpp"

namespace CustomizedSimpleGUI
{

	Pulldown* Pulldown::currentOpeningListBox = nullptr;

	void Pulldown::closeCurrentOpeningListBox()
	{
		if (Pulldown::currentOpeningListBox != nullptr)
		{
			Pulldown::currentOpeningListBox->m_isOpen = false;
			Pulldown::currentOpeningListBox = nullptr;
		}
	}

	// これを使う場合はシーンのコンストラクタで必ずPulldown::setItems()を実行すること
	Pulldown::Pulldown(const Font font, const Vec2& displayPos, const double width, const size_t listCount)
		: m_font(font)
		, m_displayPos(displayPos)
		, m_listCount(listCount)
	{
		m_isOpen = false;
		m_state.selectedItemIndex = 0;
		m_fontHeight = m_font.height();
		m_displayRegion = { m_displayPos, Max(width, 40.0), font.height() + (ListBoxFrameThickness * 2) };
		double height = (m_fontHeight + (ListBoxFrameThickness * 2)) * m_listCount;
		m_maxLines = (static_cast<int32>(height) - (ListBoxFrameThickness * 2)) / m_fontHeight;
		m_lines = Min<int32>(m_maxLines, static_cast<int32>(m_state.items.size()));
		hasScrollBar = (m_maxLines < static_cast<int32>(m_state.items.size()));
		Optional<size_t> oldState = m_state.selectedItemIndex;
		m_listRegion = {
			m_displayRegion.pos.movedBy(0, m_displayRegion.h),
			width,
			Max<double>(height, font.height() + (ListBoxFrameThickness * 2))
		};
		m_listPos = m_listRegion.pos.movedBy(ListBoxFrameThickness, ListBoxFrameThickness);
		itemWidth = static_cast<int32>(width - (ListBoxFrameThickness * 2) - (hasScrollBar ? ScrollBarWidth : 0));
	}

	Pulldown::Pulldown(ListBoxState state, const Font font, const Vec2& displayPos, const double width, const size_t listCount)
		: m_state(state)
		, m_font(font)
		, m_displayPos(displayPos)
		, m_listCount(listCount)
	{
		m_isOpen = false;
		m_state.selectedItemIndex = 0;
		m_fontHeight = m_font.height();
		m_displayRegion = { m_displayPos, Max(width, 40.0), font.height() + (ListBoxFrameThickness * 2) };
		double height = (m_fontHeight + (ListBoxFrameThickness * 2)) * m_listCount;
		m_maxLines = (static_cast<int32>(height) - (ListBoxFrameThickness * 2)) / m_fontHeight;
		m_lines = Min<int32>(m_maxLines, static_cast<int32>(m_state.items.size()));
		hasScrollBar = (m_maxLines < static_cast<int32>(m_state.items.size()));
		Optional<size_t> oldState = m_state.selectedItemIndex;
		m_listRegion = {
			m_displayRegion.pos.movedBy(0, m_displayRegion.h),
			width,
			Max<double>(height, font.height() + (ListBoxFrameThickness * 2))
		};
		m_listPos = m_listRegion.pos.movedBy(ListBoxFrameThickness, ListBoxFrameThickness);
		itemWidth = static_cast<int32>(width - (ListBoxFrameThickness * 2) - (hasScrollBar ? ScrollBarWidth : 0));
	}

	void Pulldown::emplace_back(String item)
	{
		m_state.items.emplace_back(item);
		m_lines = Min<int32>(m_maxLines, static_cast<int32>(m_state.items.size()));
		hasScrollBar = (m_maxLines < static_cast<int32>(m_state.items.size()));
		itemWidth = static_cast<int32>(m_listRegion.w - (ListBoxFrameThickness * 2) - (hasScrollBar ? ScrollBarWidth : 0));
	}

	void Pulldown::setItems(const Array<String>& items)
	{
		m_state.items = items;
		m_lines = Min<int32>(m_maxLines, static_cast<int32>(m_state.items.size()));
		hasScrollBar = (m_maxLines < static_cast<int32>(m_state.items.size()));
		itemWidth = static_cast<int32>(m_listRegion.w - (ListBoxFrameThickness * 2) - (hasScrollBar ? ScrollBarWidth : 0));
	}

	void Pulldown::setIndex(const size_t index)
	{
		m_state.selectedItemIndex = index;
	}

	void Pulldown::setIndexFromItem(const String& item)
	{
		for (size_t i = 0; i < m_state.items.size(); ++i)
		{
			if (m_state.items[i] == item)
			{
				m_state.selectedItemIndex = i;
				return;
			}
		}
	}

	size_t Pulldown::getSelectedIndex() const
	{
		return *m_state.selectedItemIndex;
	}

	size_t Pulldown::getItemsCount() const
	{
		return m_state.items.size();
	}

	String Pulldown::getSelectedItem() const
	{
		if (m_state.selectedItemIndex)
		{
			return m_state.items[*m_state.selectedItemIndex];
		}
		else
		{
			return String();
		}
	}

	bool Pulldown::getIsOpen() const
	{
		return m_isOpen;
	}

	void Pulldown::update()
	{
		if (m_state.items.size() == 0) return;

		oldState = m_state.selectedItemIndex;

		// クリックしたとき
		if (m_displayRegion.leftClicked())
		{
			// もしほかのリストボックスが開いていたら更新しない
			if (currentOpeningListBox != nullptr && currentOpeningListBox != this)
			{
				return;
			}
			m_isOpen = !m_isOpen;
			if (m_isOpen)
			{
				currentOpeningListBox = this;
			}
			else
			{
				currentOpeningListBox = nullptr;
			}
		}

		if (not m_isOpen) return;


		// リストボックスの更新

		for (int32 i = 0; i < m_lines; ++i)
		{
			const size_t itemIndex = (m_state.scroll + i);
			const RectF itemRect(m_listPos.x + ListBoxFrameThickness, m_listPos.y + ListBoxFrameThickness + (m_fontHeight * i), itemWidth, m_fontHeight);

			if (itemRect.leftClicked())
			{
				m_state.active = true;
				m_state.selectedItemIndex = itemIndex;
				currentOpeningListBox = nullptr;
				m_isOpen = false;
			}
		}

		if (MouseL.down() && not m_listRegion.mouseOver())
		{
			m_state.active = false;
		}

		if (m_state.active && m_state.selectedItemIndex)
		{
			if ((0 < *m_state.selectedItemIndex) &&
				(KeyUp.down() || ((SecondsF{ 0.33 } < KeyUp.pressedDuration()) && (SecondsF{ 0.06 } < m_state.upPressStopwatch))))
			{
				--(*m_state.selectedItemIndex);
				m_state.upPressStopwatch.restart();

				if (hasScrollBar && (static_cast<int32>(*m_state.selectedItemIndex) < m_state.scroll))
				{
					m_state.scroll = Max(m_state.scroll - 1, 0);
				}
			}
			else if (((*m_state.selectedItemIndex + 1) < m_state.items.size()) &&
				(KeyDown.down() || ((SecondsF{ 0.33 } < KeyDown.pressedDuration()) && (SecondsF{ 0.06 } < m_state.downPressStopwatch))))
			{
				++(*m_state.selectedItemIndex);
				m_state.downPressStopwatch.restart();

				if (hasScrollBar && (m_maxLines <= (static_cast<int32>(*m_state.selectedItemIndex) - m_state.scroll)))
				{
					m_state.scroll = Min(m_state.scroll + 1, static_cast<int32>(m_state.items.size()) - m_maxLines);
				}
			}
		}

		if (hasScrollBar)
		{
			const RectF scrollBarArea(m_listPos.x + m_listRegion.w - (ListBoxFrameThickness + ScrollBarWidth), m_listPos.y + ListBoxFrameThickness, ScrollBarWidth, m_listRegion.h - (ListBoxFrameThickness * 2));
			const RectF upButton(scrollBarArea.pos, ScrollBarWidth, ScrollBarWidth);
			const RectF downButton(scrollBarArea.x, scrollBarArea.y + scrollBarArea.h - ScrollBarWidth, ScrollBarWidth, ScrollBarWidth);

			if (upButton.leftClicked())
			{
				m_state.scroll = Max(m_state.scroll - 1, 0);
			}

			if (downButton.leftClicked())
			{
				m_state.scroll = Min(m_state.scroll + 1, static_cast<int32>(m_state.items.size()) - m_maxLines);
			}

			m_state.scroll = Min(m_state.scroll, static_cast<int32>(m_state.items.size()) - m_maxLines);

			const RectF innerScrollBarArea = scrollBarArea.stretched(0, -ScrollBarWidth);
			const double scrollBarHeight = (innerScrollBarArea.h * (static_cast<double>(m_maxLines) / m_state.items.size()));
			const int32 scrollBarOffset = static_cast<int32>((innerScrollBarArea.h - scrollBarHeight) * (static_cast<double>(m_state.scroll) / (m_state.items.size() - m_maxLines)));
			const RectF scrollBar(innerScrollBarArea.pos.movedBy(0, (scrollBarOffset - 3.0)), innerScrollBarArea.w, (scrollBarHeight + 6.0));

			if (m_state.scrollBarGrabbed && MouseL.up())
			{
				m_state.scrollBarGrabbed.reset();
			}

			if ((m_listRegion.mouseOver() && Mouse::Wheel()) || m_state.scrollBarGrabbed)
			{
				if (m_listRegion.mouseOver() && Mouse::Wheel())
				{
					m_state.wheel += Mouse::Wheel();
				}

				if (m_state.scrollBarGrabbed)
				{
					const double ih = static_cast<double>(innerScrollBarArea.h) / m_state.items.size();
					m_state.wheel = m_state.scrollBarGrabbed->second + ((Cursor::Pos().y - m_state.scrollBarGrabbed->first) / ih);
				}

				const double maxWheel = static_cast<int32>(m_state.items.size()) - m_maxLines;
				m_state.wheel = Clamp(m_state.wheel, 0.0, maxWheel);
				m_state.scroll = static_cast<int32>(m_state.wheel);
			}
			else
			{
				m_state.wheel = m_state.scroll;
			}

			if (scrollBar.leftClicked())
			{
				m_state.scrollBarGrabbed = std::pair{ Cursor::Pos().y, m_state.wheel };
			}
		}
		else
		{
			m_state.scrollBarGrabbed.reset();
			m_state.scroll = 0;
			m_state.wheel = 0.0;
		}
	}

	void Pulldown::draw() const
	{
		if (m_state.items.size() == 0) return;

		m_displayRegion.draw().drawFrame(DisplayRegionFrameThickness, 0, m_isOpen ? Palette::Orange : Palette::Gray);

		m_font(m_state.items[*m_state.selectedItemIndex]).draw(Arg::leftCenter(m_displayRegion.leftCenter().x + 5, m_displayRegion.leftCenter().y), Palette::Black);

		Triangle{ m_displayRegion.rightCenter().movedBy(-20, 0), (m_displayRegion.h / 3.0), 180_deg }.draw(Palette::Black);

		if (not m_isOpen) return;

		// リストボックスの描画
		{
			constexpr int32 PaddingLeft = 8;
			constexpr int32 PaddingRight = 8;

			m_listRegion
				.draw()
				.drawFrame(1, 0, ColorF{ 0.5 });

			const int32 textMaxWidth = itemWidth - (PaddingLeft + PaddingRight);
			const ColorF textColor = ActiveTextColor;

			for (int32 i = 0; i < m_lines; ++i)
			{
				const size_t itemIndex = (m_state.scroll + i);
				const bool selected = (itemIndex == m_state.selectedItemIndex);
				const RectF itemRect(m_listPos.x + ListBoxFrameThickness, m_listPos.y + ListBoxFrameThickness + (m_fontHeight * i), itemWidth, m_fontHeight);
				const RectF textRect(itemRect.pos.movedBy(PaddingLeft, 0), textMaxWidth, itemRect.h);
				const String& text = m_state.items[itemIndex];
				if (itemRect.mouseOver())
				{
					itemRect.draw(ColorF{ 0.85 });
				}
				if (selected)
				{
					const ColorF selectedItemBackgroundColor = ListBoxSelectedColor;

					itemRect
						.stretched(-1, 0, 0, -1)
						.draw(selectedItemBackgroundColor);

					m_font(text).draw(textRect, ColorF{ 1.0 });
				}
				else
				{
					m_font(text).draw(textRect, textColor);
				}
			}

			if (hasScrollBar)
			{
				const RectF scrollBarArea(m_listPos.x + m_listRegion.w - (ListBoxFrameThickness + ScrollBarWidth), m_listPos.y + ListBoxFrameThickness, ScrollBarWidth, m_listRegion.h - (ListBoxFrameThickness * 2));
				const RectF upButton(scrollBarArea.pos, ScrollBarWidth, ScrollBarWidth);
				const RectF downButton(scrollBarArea.x, scrollBarArea.y + scrollBarArea.h - ScrollBarWidth, ScrollBarWidth, ScrollBarWidth);

				scrollBarArea.draw(ColorF(0.85));

				{
					const bool pressed = ((not m_state.scrollBarGrabbed) && upButton.leftPressed());
					const Vec2 c = upButton.center();

					if (pressed)
					{
						upButton
							.stretched(-1)
							.draw(ColorF{ 0.33 });
					}

					Triangle{ c.movedBy(0, -ScrollBarWidth * 0.22),
						c.movedBy(ScrollBarWidth * 0.25, ScrollBarWidth * 0.08),
						c.movedBy(-ScrollBarWidth * 0.25, ScrollBarWidth * 0.08) }
					.draw(pressed ? ColorF{ 0.85 } : ColorF{ 0.33 });
				}

				{
					const bool pressed = ((not m_state.scrollBarGrabbed) && downButton.leftPressed());
					const Vec2 c = downButton.center();

					if (pressed)
					{
						downButton
							.stretched(-1)
							.draw(ColorF{ 0.33 });
					}

					Triangle{ c.movedBy(0, ScrollBarWidth * 0.22),
						c.movedBy(-ScrollBarWidth * 0.25, -ScrollBarWidth * 0.08),
						c.movedBy(ScrollBarWidth * 0.25, -ScrollBarWidth * 0.08) }
					.draw(pressed ? ColorF{ 0.85 } : ColorF{ 0.33 });
				}

				const RectF innerScrollBarArea = scrollBarArea.stretched(0, -ScrollBarWidth);
				const double scrollBarHeight = (innerScrollBarArea.h * (static_cast<double>(m_maxLines) / m_state.items.size()));
				const int32 scrollBarOffset = static_cast<int32>((innerScrollBarArea.h - scrollBarHeight) * (static_cast<double>(m_state.scroll) / (m_state.items.size() - m_maxLines)));
				const RectF scrollBar(innerScrollBarArea.pos.movedBy(0, (scrollBarOffset - 3.0)), innerScrollBarArea.w, (scrollBarHeight + 6.0));

				scrollBar
					.stretched(-1, 0)
					.draw(m_state.scrollBarGrabbed ? ColorF{ 0.33 } : ColorF{ 0.67 });
			}
		}
	}

	bool Pulldown::isChanged() const
	{
		return (oldState && oldState != m_state.selectedItemIndex);
	}

	RectF Pulldown::getDisplayRegion() const
	{
		return m_displayRegion;
	}

}
