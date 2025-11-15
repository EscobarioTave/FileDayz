class MarkersListAdapter extends ListAdapter {

    private ref array<ref HeaderItem> m_HeaderItems = new array<ref HeaderItem>>;

    private ref HeaderItem m_ServerMarkersHeaderItem;
    private ref HeaderItem m_CustomServerMarkersHeaderItem;
    private ref HeaderItem m_ClientMarkersHeaderItem;

    VPPMapMenu m_Menu;

    void MarkersListAdapter(VPPMapMenu menu, Widget root) {
        super.init("VanillaPPMap/GUI/Layouts/List/VerticalList.layout", root);

        m_Menu = menu;

        m_ServerMarkersHeaderItem = new HeaderItem("Server Markers");
        m_HeaderItems.Insert(m_ServerMarkersHeaderItem);
        m_CustomServerMarkersHeaderItem = new HeaderItem("My Server Markers");
        m_HeaderItems.Insert(m_CustomServerMarkersHeaderItem);
        m_ClientMarkersHeaderItem = new HeaderItem("My Global Markers");
        m_HeaderItems.Insert(m_ClientMarkersHeaderItem);
    }

    HeaderItem GetCustomServerMarkersHeaderItem() {
        return m_CustomServerMarkersHeaderItem;
    }

    HeaderItem GetServerMarkersHeaderItem() {
        return m_ServerMarkersHeaderItem;
    }

    HeaderItem GetClientMarkersHeaderItem() {
        return m_ClientMarkersHeaderItem;
    }

    void UpdateContent() {
        array<ref VPPMapItem> items = new array<ref VPPMapItem>>;
        foreach (HeaderItem headerItem : m_HeaderItems) {
            items.Insert(headerItem);
            if (headerItem.m_IsExpanded) {
                foreach (ListItem listItem : headerItem.m_ListItems) {
                    items.Insert(listItem);
                }
            }
        }
        super.UpdateContent(items);
    }

    override void AddAllWidgets(array<ref VPPMapItem> items) {
        Widget listContentContainer;
        int itemCounter = 0;
        foreach (VPPMapItem item: items) {
            itemCounter++;
            if (!listContentContainer || itemCounter >= 99) {
                itemCounter = 0;
                listContentContainer = GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/List/ListContentContainer.layout", m_ListContent);
            }
            ItemViewHolder viewHolder = CreateViewHolder(listContentContainer, item);
            FillViewHolder(viewHolder, item);
            m_WidgetItemMap.Insert(viewHolder.m_RootWidget, item);
        }
    }

    override ItemViewHolder CreateViewHolder(Widget root, VPPMapItem item) {
        if (item.IsInherited(HeaderItem)) {
            return HeaderItemViewHolder(root);
        } else if (item.IsInherited(ListItem)) {
            return ListItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(ItemViewHolder viewHolder, VPPMapItem item) {
        if (item.IsInherited(HeaderItem) && viewHolder.IsInherited(HeaderItemViewHolder)) {
            HeaderItemViewHolder hivh = HeaderItemViewHolder.Cast(viewHolder);
            HeaderItem headerItem = HeaderItem.Cast(item);
            hivh.fillView(headerItem);
        } else if (item.IsInherited(ListItem) && viewHolder.IsInherited(ListItemViewHolder)) {
            ListItemViewHolder livh = ListItemViewHolder.Cast(viewHolder);
            ListItem listitem = ListItem.Cast(item);
            livh.fillView(listitem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentHeight() > y;
    }

    override void OnItemClicked(string widgetName, VPPMapItem item) {

        HeaderItem headerItem;
        if (widgetName == "collapse_icon" || widgetName == "expand_icon") {
            if (item.IsInherited(HeaderItem)) {
                headerItem = HeaderItem.Cast(item);
                headerItem.ToggleExpandCollapse();
                UpdateContent();
            }
        } else if (widgetName == "visible_button") {
            array<VerticalItem> items = new array<VerticalItem>;
            if (item.IsInherited(HeaderItem)) { // User clicked a HeaderItem
                headerItem = HeaderItem.Cast(item);
                items.Insert(headerItem);
                foreach (ListItem listItem : headerItem.m_ListItems) { // Add all children
                    items.Insert(listItem);
                }
            } else if (item.IsInherited(ListItem)) { // User clicked a ListItem
                items.Insert(VerticalItem.Cast(item));
            }
            if (items.Count() > 0 && item.IsInherited(VerticalItem)) {
                VerticalItem verticalItem = VerticalItem.Cast(item);
                switch (verticalItem.m_VisibleState) {
                case VisibleState.NOT_VISIBLE:
                    SetVisible(items, VisibleState.VISIBLE_ONLY_2D);
                    break;
                case VisibleState.VISIBLE_ONLY_2D:
                    SetVisible(items, VisibleState.VISIBLE_2D_AND_3D);
                    break;
                case VisibleState.VISIBLE_2D_AND_3D:
                    SetVisible(items, VisibleState.NOT_VISIBLE);
                    break;
                }
            }
        } else if ((widgetName == "image" || widgetName == "text") && item.IsInherited(ListItem)) {
            ListItem list_item = ListItem.Cast(item);
            if (list_item.m_HeaderParent == GetClientMarkersHeaderItem()) {
                m_Menu.ShowDialog(list_item.m_MarkerIndex, false, false);
            } else if (list_item.m_HeaderParent == GetCustomServerMarkersHeaderItem()) {
                m_Menu.ShowDialog(list_item.m_MarkerIndex, true, false);
            }
        }
    }

    void SetVisible(array<VerticalItem> items, VisibleState visibleState) {
        array<int> markerIndexes = new array<int>;
        bool isClientMarker = false;
        bool isCustomServerMarker = false;
        foreach (VerticalItem item : items) {
            item.SetVisible(visibleState);
            if (item.IsInherited(ListItem)) {
                ListItem listItem = ListItem.Cast(item);
                if (listItem.m_HeaderParent == GetClientMarkersHeaderItem()) {
                    isClientMarker = true;
                } else if (listItem.m_HeaderParent == GetCustomServerMarkersHeaderItem()) {
                    isCustomServerMarker = true;
                }
                markerIndexes.Insert(listItem.m_MarkerIndex);
            }
        }
        switch (visibleState) {
        case VisibleState.NOT_VISIBLE:
            m_Menu.EditMarkerVisibility(markerIndexes, false, false, isClientMarker, isCustomServerMarker);
            break;
        case VisibleState.VISIBLE_ONLY_2D:
            m_Menu.EditMarkerVisibility(markerIndexes, true, false, isClientMarker, isCustomServerMarker);
            break;
        case VisibleState.VISIBLE_2D_AND_3D:
            m_Menu.EditMarkerVisibility(markerIndexes, true, true, isClientMarker, isCustomServerMarker);
            break;
        }
    }
}

enum VisibleState {
    NOT_VISIBLE,
    VISIBLE_ONLY_2D,
    VISIBLE_2D_AND_3D
}

class VerticalItemViewHolder extends ItemViewHolder {
    TextWidget m_TextWidget;
    ImageWidget m_VisibleButton;

    override void init( Widget root) {
        super.init(root);

        m_TextWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget( "text" ) );
        m_VisibleButton = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "visible_button" ) );
    }

    void fillView(VerticalItem item) {
        switch (item.m_VisibleState) {
        case VisibleState.NOT_VISIBLE:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_not_visible");
            break;
        case VisibleState.VISIBLE_ONLY_2D:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_visible");
            break;
        case VisibleState.VISIBLE_2D_AND_3D:
            m_VisibleButton.LoadImageFile(0, "set:vpp_map_ui image:icon_visible_3d");
            break;
        }
    }
}

class VerticalItem extends VPPMapItem {
    string m_Text;
    VisibleState m_VisibleState = VisibleState.VISIBLE_2D_AND_3D;

    void SetVisible(VisibleState visibleState) {
        m_VisibleState = visibleState;
    }
}

class HeaderItemViewHolder extends VerticalItemViewHolder {
    ImageWidget m_CollapseIcon;
    ImageWidget m_ExpandIcon;

    void HeaderItemViewHolder(Widget root) {
        init(GetGame().GetWorkspace().CreateWidgets( "VanillaPPMap/GUI/Layouts/List/MarkerHeaderItem.layout", root));

        m_CollapseIcon = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "collapse_icon" ) );
        m_ExpandIcon = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "expand_icon" ) );
    }

    void fillView(HeaderItem headerItem) {
        super.fillView(headerItem);

        m_TextWidget.SetText(headerItem.m_Text);
        m_CollapseIcon.LoadImageFile(0, "set:vpp_map_ui image:icon_expand");
        m_ExpandIcon.LoadImageFile(0, "set:vpp_map_ui image:icon_collapse");
        if (headerItem.m_IsExpanded) {
            m_CollapseIcon.Show(true);
            m_ExpandIcon.Show(false);
        } else {
            m_CollapseIcon.Show(false);
            m_ExpandIcon.Show(true);
        }
    }
}

class HeaderItem extends VerticalItem {
    bool m_IsExpanded = true;
    ref array<ref ListItem> m_ListItems = new array<ref ListItem>>;

    void HeaderItem(string text) {
        m_Text = text;
    }

    void ToggleExpandCollapse() {
        m_IsExpanded = !m_IsExpanded;
    }

    void AddListItem(ref ListItem listItem) {
        m_ListItems.Insert(listItem);
    }

    void RemoveListItem(ref ListItem listItem) {
        m_ListItems.RemoveItem(listItem);
    }

    void Clear() {
        m_ListItems = new array<ref ListItem>>;
    }
}

class ListItemViewHolder extends VerticalItemViewHolder {
    ImageWidget m_Image;

    void ListItemViewHolder(Widget root) {
        init(GetGame().GetWorkspace().CreateWidgets( "VanillaPPMap/GUI/Layouts/List/MarkerListItem.layout", root));

        m_Image = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "image" ) );
    }

    void fillView(ListItem listItem) {
        super.fillView(listItem);

        m_TextWidget.SetText(listItem.m_Text);
        m_TextWidget.SetColor(VPPMapMenu.VectorToARGB(listItem.m_Marker.GetColor()));
        m_Image.LoadImageFile(0, listItem.m_MarkerIconPath);
        m_Image.SetColor(VPPMapMenu.VectorToARGB(listItem.m_Marker.GetColor()));
    }
}

class ListItem extends VerticalItem {
    ref MarkerInfo m_Marker;
    string m_MarkerIconPath;
    int m_MarkerIndex;
    ref HeaderItem m_HeaderParent;

    void ListItem(HeaderItem headerParent, MarkerInfo marker, int markerIndex) {
        m_HeaderParent = headerParent;
        m_Marker = marker;
        m_MarkerIndex = markerIndex;
        m_Text = marker.GetName();
        m_MarkerIconPath = marker.GetIconPath();
        if (marker.IsActive()) {
            if (marker.Is3DActive()) {
                SetVisible(VisibleState.VISIBLE_2D_AND_3D);
            } else {
                SetVisible(VisibleState.VISIBLE_ONLY_2D);
            }
        } else {
            SetVisible(VisibleState.NOT_VISIBLE);
        }
    }
}