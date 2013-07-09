/*
 *  DA probe
 *
 * Copyright (c) 2000 - 2013 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact:
 *
 * Jaewon Lim <jaewon81.lim@samsung.com>
 * Woojin Jung <woojin2.jung@samsung.com>
 * Juyoung Kim <j0.kim@samsung.com>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Contributors:
 * - S-Core Co., Ltd
 *
 */

#include "daprobe.h"
#include "dahelper.h"
#include "dacollection.h"
#include "osp_probe.h"

namespace Tizen { namespace Ui {

class Control
{
	Control(void);
	~Control(void);
};

class CustomControlBase
{
	CustomControlBase(void);
};

class Container
{
	Container(void);
};

class Window
{
	Window(void);
};

Control::Control(void)
{
	typedef void (Control::*methodType)(void);
	static methodType control_controlp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui7ControlC2Ev, LIBOSP_UIFW, control_controlp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Control");
	probeBlockEnd();

	(this->*control_controlp)();
}

Control::~Control(void)
{
	typedef void (Control::*methodType)(void);
	static methodType control__controlvoidp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui7ControlD2Ev, LIBOSP_UIFW, control__controlvoidp);

	probeBlockStart();
	del_object_hash(static_cast<void*>(this));
	probeBlockEnd();

	(this->*control__controlvoidp)();
}

CustomControlBase::CustomControlBase(void)
{
	typedef void (CustomControlBase::*methodType)(void);
	static methodType customcontrolbase_customcontrolbasep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui17CustomControlBaseC2Ev, LIBOSP_UIFW, customcontrolbase_customcontrolbasep);

	probeBlockStart();
	add_object_hash_type((void*)this, "CustomControlBase");
	probeBlockEnd();

	(this->*customcontrolbase_customcontrolbasep)();
}

Container::Container(void)
{
	typedef void (Container::*methodType)(void);
	static methodType container_containerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui9ContainerC2Ev, LIBOSP_UIFW, container_containerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Container");
	probeBlockEnd();

	(this->*container_containerp)();
}

Window::Window(void)
{
	typedef void (Window::*methodType)(void);
	static methodType window_windowp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui6WindowC2Ev, LIBOSP_UIFW, window_windowp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Window");
	probeBlockEnd();

	(this->*window_windowp)();
}

namespace Controls {

class Animation
{
	Animation(void);
};

class Button
{
	Button(void);
};

class CheckButton
{
	CheckButton(void);
};

class ColorPicker
{
	ColorPicker(void);
};

class CustomList
{
	CustomList(void);
};

class EditArea
{
	EditArea(void);
};

class EditDate
{
	EditDate(void);
};

class EditField
{
	EditField(void);
};

class EditTime
{
	EditTime(void);
};

class ExpandableEditArea
{
	ExpandableEditArea(void);
};

class ExpandableList
{
	ExpandableList(void);
};

class Footer
{
	Footer(void);
};

class Gallery
{
	Gallery(void);
};

class GroupedList
{
	GroupedList(void);
};

class GroupedListView
{
	GroupedListView(void);
};

class Header
{
	Header(void);
};

class IconList
{
	IconList(void);
};

class IconListView
{
	IconListView(void);
};

class Label
{
	Label(void);
};

class List
{
	List(void);
};

class ListView
{
	ListView(void);
};

class Progress
{
	Progress(void);
};

class RadioGroup
{
	RadioGroup(void);
};

class SearchBar
{
	SearchBar(void);
};

class SlidableGroupedList
{
	SlidableGroupedList(void);
};

class SlidableList
{
	SlidableList(void);
};

class Slider
{
	Slider(void);
};

class SplitPanel
{
	SplitPanel(void);
};

class Tab
{
	Tab(void);
};

class TabBar
{
	TabBar(void);
};

class TextBox
{
	TextBox(void);
};

class Form
{
	Form(void);
};

class GroupedTableView
{
	GroupedTableView(void);
};

class Panel
{
	Panel(void);
};

class OverlayPanel
{
	OverlayPanel(void);
};

class ScrollPanel
{
	ScrollPanel(void);
};

class SectionTableView
{
	SectionTableView(void);
};

class TableView
{
	TableView(void);
};

class TableViewItemBase
{
	TableViewItemBase(void);
};

class TableViewContextItem
{
	TableViewContextItem(void);
};

class TableViewGroupItem
{
	TableViewGroupItem(void);
};

class TableViewItem
{
	TableViewItem(void);
};

class TableViewSimpleGroupItem
{
	TableViewSimpleGroupItem(void);
};

class TableViewSimpleItem
{
	TableViewSimpleItem(void);
};

class ContextMenu
{
	ContextMenu(void);
};

class DatePicker
{
	DatePicker(void);
};

class DateTimePicker
{
	DateTimePicker(void);
};

class Frame
{
	Frame(void);
};

class Keypad
{
	Keypad(void);
};

class MessageBox
{
	MessageBox(void);
};

class NotificationFrame
{
	NotificationFrame(void);
};

class OptionMenu
{
	OptionMenu(void);
};

class Popup
{
	Popup(void);
};

class TimePicker
{
	TimePicker(void);
};

Animation::Animation(void)
{
	typedef void (Animation::*methodType)(void);
	static methodType animation_animationp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls9AnimationC2Ev, LIBOSP_UIFW, animation_animationp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Animation");
	probeBlockEnd();

	(this->*animation_animationp)();
}

Button::Button(void)
{
	typedef void (Button::*methodType)(void);
	static methodType button_buttonp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6ButtonC2Ev, LIBOSP_UIFW, button_buttonp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Button");
	probeBlockEnd();

	(this->*button_buttonp)();
}

CheckButton::CheckButton(void)
{
	typedef void (CheckButton::*methodType)(void);
	static methodType checkbutton_checkbuttonp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls11CheckButtonC2Ev, LIBOSP_UIFW, checkbutton_checkbuttonp);

	probeBlockStart();
	add_object_hash_type((void*)this, "CheckButton");
	probeBlockEnd();

	(this->*checkbutton_checkbuttonp)();
}

ColorPicker::ColorPicker(void)
{
	typedef void (ColorPicker::*methodType)(void);
	static methodType colorpicker_colorpickerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls11ColorPickerC2Ev, LIBOSP_UIFW, colorpicker_colorpickerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "ColorPicker");
	probeBlockEnd();

	(this->*colorpicker_colorpickerp)();
}

CustomList::CustomList(void)
{
	typedef void (CustomList::*methodType)(void);
	static methodType customlist_customlistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10CustomListC2Ev, LIBOSP_UIFW, customlist_customlistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "CustomList");
	probeBlockEnd();

	(this->*customlist_customlistp)();
}

EditArea::EditArea(void)
{
	typedef void (EditArea::*methodType)(void);
	static methodType editarea_editareap;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8EditAreaC2Ev, LIBOSP_UIFW, editarea_editareap);

	probeBlockStart();
	add_object_hash_type((void*)this, "EditArea");
	probeBlockEnd();

	(this->*editarea_editareap)();
}

EditDate::EditDate(void)
{
	typedef void (EditDate::*methodType)(void);
	static methodType editdate_editdatep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8EditDateC2Ev, LIBOSP_UIFW, editdate_editdatep);

	probeBlockStart();
	add_object_hash_type((void*)this, "EditDate");
	probeBlockEnd();

	(this->*editdate_editdatep)();
}

EditField::EditField(void)
{
	typedef void (EditField::*methodType)(void);
	static methodType editfield_editfieldp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls9EditFieldC2Ev, LIBOSP_UIFW, editfield_editfieldp);

	probeBlockStart();
	add_object_hash_type((void*)this, "EditField");
	probeBlockEnd();

	(this->*editfield_editfieldp)();
}

EditTime::EditTime(void)
{
	typedef void (EditTime::*methodType)(void);
	static methodType edittime_edittimep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8EditTimeC2Ev, LIBOSP_UIFW, edittime_edittimep);

	probeBlockStart();
	add_object_hash_type((void*)this, "EditTime");
	probeBlockEnd();

	(this->*edittime_edittimep)();
}

ExpandableEditArea::ExpandableEditArea(void)
{
	typedef void (ExpandableEditArea::*methodType)(void);
	static methodType expandableeditarea_expandableeditareap;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls18ExpandableEditAreaC2Ev, LIBOSP_UIFW, expandableeditarea_expandableeditareap);

	probeBlockStart();
	add_object_hash_type((void*)this, "ExpandableEditArea");
	probeBlockEnd();

	(this->*expandableeditarea_expandableeditareap)();
}

ExpandableList::ExpandableList(void)
{
	typedef void (ExpandableList::*methodType)(void);
	static methodType expandablelist_expandablelistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls14ExpandableListC2Ev, LIBOSP_UIFW, expandablelist_expandablelistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "ExpandableList");
	probeBlockEnd();

	(this->*expandablelist_expandablelistp)();
}

Footer::Footer(void)
{
	typedef void (Footer::*methodType)(void);
	static methodType footer_footerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6FooterC2Ev, LIBOSP_UIFW, footer_footerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Footer");
	probeBlockEnd();

	(this->*footer_footerp)();
}

Gallery::Gallery(void)
{
	typedef void (Gallery::*methodType)(void);
	static methodType gallery_galleryp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls7GalleryC2Ev, LIBOSP_UIFW, gallery_galleryp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Gallery");
	probeBlockEnd();

	(this->*gallery_galleryp)();
}

GroupedList::GroupedList(void)
{
	typedef void (GroupedList::*methodType)(void);
	static methodType groupedlist_groupedlistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls11GroupedListC2Ev, LIBOSP_UIFW, groupedlist_groupedlistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "GroupedList");
	probeBlockEnd();

	(this->*groupedlist_groupedlistp)();
}

GroupedListView::GroupedListView(void)
{
	typedef void (GroupedListView::*methodType)(void);
	static methodType groupedlistview_groupedlistviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls15GroupedListViewC2Ev, LIBOSP_UIFW, groupedlistview_groupedlistviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "GroupedListView");
	probeBlockEnd();

	(this->*groupedlistview_groupedlistviewp)();
}

Header::Header(void)
{
	typedef void (Header::*methodType)(void);
	static methodType header_headerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6HeaderC2Ev, LIBOSP_UIFW, header_headerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Header");
	probeBlockEnd();

	(this->*header_headerp)();
}

IconList::IconList(void)
{
	typedef void (IconList::*methodType)(void);
	static methodType iconlist_iconlistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8IconListC2Ev, LIBOSP_UIFW, iconlist_iconlistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "IconList");
	probeBlockEnd();

	(this->*iconlist_iconlistp)();
}

IconListView::IconListView(void)
{
	typedef void (IconListView::*methodType)(void);
	static methodType iconlistview_iconlistviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls12IconListViewC2Ev, LIBOSP_UIFW, iconlistview_iconlistviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "IconListView");
	probeBlockEnd();

	(this->*iconlistview_iconlistviewp)();
}

Label::Label(void)
{
	typedef void (Label::*methodType)(void);
	static methodType label_labelp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls5LabelC2Ev, LIBOSP_UIFW, label_labelp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Label");
	probeBlockEnd();

	(this->*label_labelp)();
}

List::List(void)
{
	typedef void (List::*methodType)(void);
	static methodType list_listp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls4ListC2Ev, LIBOSP_UIFW, list_listp);

	probeBlockStart();
	add_object_hash_type((void*)this, "List");
	probeBlockEnd();

	(this->*list_listp)();
}

ListView::ListView(void)
{
	typedef void (ListView::*methodType)(void);
	static methodType listview_listviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8ListViewC2Ev, LIBOSP_UIFW, listview_listviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "ListView");
	probeBlockEnd();

	(this->*listview_listviewp)();
}

Progress::Progress(void)
{
	typedef void (Progress::*methodType)(void);
	static methodType progress_progressp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls8ProgressC2Ev, LIBOSP_UIFW, progress_progressp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Progress");
	probeBlockEnd();

	(this->*progress_progressp)();
}

RadioGroup::RadioGroup(void)
{
	typedef void (RadioGroup::*methodType)(void);
	static methodType radiogroup_radiogroupp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10RadioGroupC2Ev, LIBOSP_UIFW, radiogroup_radiogroupp);

	probeBlockStart();
	add_object_hash_type((void*)this, "RadioGroup");
	probeBlockEnd();

	(this->*radiogroup_radiogroupp)();
}

SearchBar::SearchBar(void)
{
	typedef void (SearchBar::*methodType)(void);
	static methodType searchbar_searchbarp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls9SearchBarC2Ev, LIBOSP_UIFW, searchbar_searchbarp);

	probeBlockStart();
	add_object_hash_type((void*)this, "SearchBar");
	probeBlockEnd();

	(this->*searchbar_searchbarp)();
}

SlidableGroupedList::SlidableGroupedList(void)
{
	typedef void (SlidableGroupedList::*methodType)(void);
	static methodType slidablegroupedlist_slidablegroupedlistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls19SlidableGroupedListC2Ev, LIBOSP_UIFW, slidablegroupedlist_slidablegroupedlistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "SlidableGroupedList");
	probeBlockEnd();

	(this->*slidablegroupedlist_slidablegroupedlistp)();
}

SlidableList::SlidableList(void)
{
	typedef void (SlidableList::*methodType)(void);
	static methodType slidablelist_slidablelistp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls12SlidableListC2Ev, LIBOSP_UIFW, slidablelist_slidablelistp);

	probeBlockStart();
	add_object_hash_type((void*)this, "SlidableList");
	probeBlockEnd();

	(this->*slidablelist_slidablelistp)();
}

Slider::Slider(void)
{
	typedef void (Slider::*methodType)(void);
	static methodType slider_sliderp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6SliderC2Ev, LIBOSP_UIFW, slider_sliderp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Slider");
	probeBlockEnd();

	(this->*slider_sliderp)();
}

SplitPanel::SplitPanel(void)
{
	typedef void (SplitPanel::*methodType)(void);
	static methodType splitpanel_splitpanelp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10SplitPanelC2Ev, LIBOSP_UIFW, splitpanel_splitpanelp);

	probeBlockStart();
	add_object_hash_type((void*)this, "SplitPanel");
	probeBlockEnd();

	(this->*splitpanel_splitpanelp)();
}

Tab::Tab(void)
{
	typedef void (Tab::*methodType)(void);
	static methodType tab_tabp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls3TabC2Ev, LIBOSP_UIFW, tab_tabp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Tab");
	probeBlockEnd();

	(this->*tab_tabp)();
}

TabBar::TabBar(void)
{
	typedef void (TabBar::*methodType)(void);
	static methodType tabbar_tabbarp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6TabBarC2Ev, LIBOSP_UIFW, tabbar_tabbarp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TabBar");
	probeBlockEnd();

	(this->*tabbar_tabbarp)();
}

TextBox::TextBox(void)
{
	typedef void (TextBox::*methodType)(void);
	static methodType textbox_textboxp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls7TextBoxC2Ev, LIBOSP_UIFW, textbox_textboxp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TextBox");
	probeBlockEnd();

	(this->*textbox_textboxp)();
}

Form::Form(void)
{
	typedef void (Form::*methodType)(void);
	static methodType form_formp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls4FormC2Ev, LIBOSP_UIFW, form_formp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Form");
	probeBlockEnd();

	(this->*form_formp)();
}

GroupedTableView::GroupedTableView(void)
{
	typedef void (GroupedTableView::*methodType)(void);
	static methodType groupedtableview_groupedtableviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls16GroupedTableViewC2Ev, LIBOSP_UIFW, groupedtableview_groupedtableviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "GroupedTableView");
	probeBlockEnd();

	(this->*groupedtableview_groupedtableviewp)();
}

Panel::Panel(void)
{
	typedef void (Panel::*methodType)(void);
	static methodType panel_panelp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls5PanelC2Ev, LIBOSP_UIFW, panel_panelp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Panel");
	probeBlockEnd();

	(this->*panel_panelp)();
}

OverlayPanel::OverlayPanel(void)
{
	typedef void (OverlayPanel::*methodType)(void);
	static methodType overlaypanel_overlaypanelp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls12OverlayPanelC2Ev, LIBOSP_UIFW, overlaypanel_overlaypanelp);

	probeBlockStart();
	add_object_hash_type((void*)this, "OverlayPanel");
	probeBlockEnd();

	(this->*overlaypanel_overlaypanelp)();
}

ScrollPanel::ScrollPanel(void)
{
	typedef void (ScrollPanel::*methodType)(void);
	static methodType scrollpanel_scrollpanelp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls11ScrollPanelC2Ev, LIBOSP_UIFW, scrollpanel_scrollpanelp);

	probeBlockStart();
	add_object_hash_type((void*)this, "ScrollPanel");
	probeBlockEnd();

	(this->*scrollpanel_scrollpanelp)();
}

SectionTableView::SectionTableView(void)
{
	typedef void (SectionTableView::*methodType)(void);
	static methodType sectiontableview_sectiontableviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls16SectionTableViewC2Ev, LIBOSP_UIFW, sectiontableview_sectiontableviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "SectionTableView");
	probeBlockEnd();

	(this->*sectiontableview_sectiontableviewp)();
}

TableView::TableView(void)
{
	typedef void (TableView::*methodType)(void);
	static methodType tableview_tableviewp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls9TableViewC2Ev, LIBOSP_UIFW, tableview_tableviewp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableView");
	probeBlockEnd();

	(this->*tableview_tableviewp)();
}

TableViewItemBase::TableViewItemBase(void)
{
	typedef void (TableViewItemBase::*methodType)(void);
	static methodType tableviewitembase_tableviewitembasep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls17TableViewItemBaseC2Ev, LIBOSP_UIFW, tableviewitembase_tableviewitembasep);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewItemBase");
	probeBlockEnd();

	(this->*tableviewitembase_tableviewitembasep)();
}

TableViewContextItem::TableViewContextItem(void)
{
	typedef void (TableViewContextItem::*methodType)(void);
	static methodType tableviewcontextitem_tableviewcontextitemp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls20TableViewContextItemC2Ev, LIBOSP_UIFW, tableviewcontextitem_tableviewcontextitemp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewContextItem");
	probeBlockEnd();

	(this->*tableviewcontextitem_tableviewcontextitemp)();
}

TableViewGroupItem::TableViewGroupItem(void)
{
	typedef void (TableViewGroupItem::*methodType)(void);
	static methodType tableviewgroupitem_tableviewgroupitemp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls18TableViewGroupItemC2Ev, LIBOSP_UIFW, tableviewgroupitem_tableviewgroupitemp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewGroupItem");
	probeBlockEnd();

	(this->*tableviewgroupitem_tableviewgroupitemp)();
}

TableViewItem::TableViewItem(void)
{
	typedef void (TableViewItem::*methodType)(void);
	static methodType tableviewitem_tableviewitemp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls13TableViewItemC2Ev, LIBOSP_UIFW, tableviewitem_tableviewitemp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewItem");
	probeBlockEnd();

	(this->*tableviewitem_tableviewitemp)();
}

TableViewSimpleGroupItem::TableViewSimpleGroupItem(void)
{
	typedef void (TableViewSimpleGroupItem::*methodType)(void);
	static methodType tableviewsimplegroupitem_tableviewsimplegroupitemp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls24TableViewSimpleGroupItemC2Ev, LIBOSP_UIFW, tableviewsimplegroupitem_tableviewsimplegroupitemp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewSimpleGroupItem");
	probeBlockEnd();

	(this->*tableviewsimplegroupitem_tableviewsimplegroupitemp)();
}

TableViewSimpleItem::TableViewSimpleItem(void)
{
	typedef void (TableViewSimpleItem::*methodType)(void);
	static methodType tableviewsimpleitem_tableviewsimpleitemp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls19TableViewSimpleItemC2Ev, LIBOSP_UIFW, tableviewsimpleitem_tableviewsimpleitemp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TableViewSimpleItem");
	probeBlockEnd();

	(this->*tableviewsimpleitem_tableviewsimpleitemp)();
}

ContextMenu::ContextMenu(void)
{
	typedef void (ContextMenu::*methodType)(void);
	static methodType contextmenu_contextmenup;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls11ContextMenuC2Ev, LIBOSP_UIFW, contextmenu_contextmenup);

	probeBlockStart();
	add_object_hash_type((void*)this, "ContextMenu");
	probeBlockEnd();

	(this->*contextmenu_contextmenup)();
}

DatePicker::DatePicker(void)
{
	typedef void (DatePicker::*methodType)(void);
	static methodType datepicker_datepickerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10DatePickerC2Ev, LIBOSP_UIFW, datepicker_datepickerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "DatePicker");
	probeBlockEnd();

	(this->*datepicker_datepickerp)();
}

DateTimePicker::DateTimePicker(void)
{
	typedef void (DateTimePicker::*methodType)(void);
	static methodType datetimepicker_datetimepickerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls14DateTimePickerC2Ev, LIBOSP_UIFW, datetimepicker_datetimepickerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "DateTimePicker");
	probeBlockEnd();

	(this->*datetimepicker_datetimepickerp)();
}

Frame::Frame(void)
{
	typedef void (Frame::*methodType)(void);
	static methodType frame_framep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls5FrameC2Ev, LIBOSP_UIFW, frame_framep);

	probeBlockStart();
	add_object_hash_type((void*)this, "Frame");
	probeBlockEnd();

	(this->*frame_framep)();
}

Keypad::Keypad(void)
{
	typedef void (Keypad::*methodType)(void);
	static methodType keypad_keypadp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls6KeypadC2Ev, LIBOSP_UIFW, keypad_keypadp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Keypad");
	probeBlockEnd();

	(this->*keypad_keypadp)();
}

MessageBox::MessageBox(void)
{
	typedef void (MessageBox::*methodType)(void);
	static methodType messagebox_messageboxp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10MessageBoxC2Ev, LIBOSP_UIFW, messagebox_messageboxp);

	probeBlockStart();
	add_object_hash_type((void*)this, "MessageBox");
	probeBlockEnd();

	(this->*messagebox_messageboxp)();
}

NotificationFrame::NotificationFrame(void)
{
	typedef void (NotificationFrame::*methodType)(void);
	static methodType notificationframe_notificationframep;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls17NotificationFrameC2Ev, LIBOSP_UIFW, notificationframe_notificationframep);

	probeBlockStart();
	add_object_hash_type((void*)this, "NotificationFrame");
	probeBlockEnd();

	(this->*notificationframe_notificationframep)();
}

OptionMenu::OptionMenu(void)
{
	typedef void (OptionMenu::*methodType)(void);
	static methodType optionmenu_optionmenup;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10OptionMenuC2Ev, LIBOSP_UIFW, optionmenu_optionmenup);

	probeBlockStart();
	add_object_hash_type((void*)this, "OptionMenu");
	probeBlockEnd();

	(this->*optionmenu_optionmenup)();
}

Popup::Popup(void)
{
	typedef void (Popup::*methodType)(void);
	static methodType popup_popupp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls5PopupC2Ev, LIBOSP_UIFW, popup_popupp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Popup");
	probeBlockEnd();

	(this->*popup_popupp)();
}

TimePicker::TimePicker(void)
{
	typedef void (TimePicker::*methodType)(void);
	static methodType timepicker_timepickerp;

	GET_REAL_FUNC_OSP(_ZN5Tizen2Ui8Controls10TimePickerC2Ev, LIBOSP_UIFW, timepicker_timepickerp);

	probeBlockStart();
	add_object_hash_type((void*)this, "TimePicker");
	probeBlockEnd();

	(this->*timepicker_timepickerp)();
}

}		// end of namespace Tizen::Ui::Controls

} }		// end of namespace Tizen::Ui


namespace Tizen { namespace Web { namespace Controls {

class Web
{
	Web(void);
};

Web::Web(void)
{
	typedef void (Web::*methodType)(void);
	static methodType web_webp;

	GET_REAL_FUNC_OSP(_ZN5Tizen3Web8Controls3WebC2Ev, LIBOSP_WEB, web_webp);

	probeBlockStart();
	add_object_hash_type((void*)this, "Web");
	probeBlockEnd();

	(this->*web_webp)();
}

} } }	// end of namespace Tizen::Web::Controls
