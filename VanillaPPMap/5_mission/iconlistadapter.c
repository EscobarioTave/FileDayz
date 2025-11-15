class IconListAdapter extends ListAdapter {

    private string m_SelectedIcon;
    private vector m_SelectedColor = Vector(255,255,255);

    void IconListAdapter(Widget root) {
        super.init("VanillaPPMap/GUI/Layouts/List/HorizontalList.layout", root);
    }

    void SetSelectedIcon(string icon) {
        m_SelectedIcon = icon;
        UpdateContent();
    }

    void SetSelectedColor(vector color) {
        m_SelectedColor = color;
        UpdateContent();
    }

    string GetSelectedIcon() {
        return m_SelectedIcon;
    }

    void UpdateContent() {
        array<ref MarkerIcon> markerIcons = MarkerOptions.GetInstance().GetAllIcons();
        array<ref VPPMapItem> items = new array<ref VPPMapItem>>;
        foreach (MarkerIcon markerIcon : markerIcons) {
            IconItem iconItem = new IconItem();
            iconItem.m_IconPath = markerIcon.getMarkerIconPath();
            iconItem.m_SelectedColor = m_SelectedColor;
            if (!m_SelectedIcon || m_SelectedIcon == iconItem.m_IconPath) {
                iconItem.m_IsSelected = true;
                m_SelectedIcon = iconItem.m_IconPath;
            }
            items.Insert(iconItem);
        }
        super.UpdateContent(items);
    }

    override void AddAllWidgets(array<ref VPPMapItem> items) {
        foreach (VPPMapItem item: items) {
            ItemViewHolder viewHolder = CreateViewHolder(m_ListContent, item);
            FillViewHolder(viewHolder, item);
            m_WidgetItemMap.Insert(viewHolder.m_RootWidget, item);
        }
    }

    override ItemViewHolder CreateViewHolder(Widget root, VPPMapItem item) {
        if (item.IsInherited(IconItem)) {
            return IconItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(ItemViewHolder viewHolder, VPPMapItem item) {
        if (item.IsInherited(IconItem) && viewHolder.IsInherited(IconItemViewHolder)) {
            IconItemViewHolder vh = IconItemViewHolder.Cast(viewHolder);
            IconItem iconItem = IconItem.Cast(item);
            vh.fillView(iconItem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentWidth() > x;
    }

    override void OnItemClicked(string widgetName, VPPMapItem item) {

        if (widgetName == "icon_image" || widgetName == "selected_image") {

            if (item.IsInherited(IconItem)) {
                IconItem iconItem = IconItem.Cast(item);
                m_SelectedIcon = iconItem.m_IconPath;
                UpdateContent();
            }
        }
    }
}

class IconItemViewHolder extends ItemViewHolder {
    ImageWidget mIconImage;
    ImageWidget mSelectedImage;

    void IconItemViewHolder(Widget root) {
        super.init(GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/List/IconListItem.layout", root));

        mIconImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "icon_image" ) );
        mSelectedImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "selected_image" ) );
    }

    void fillView(IconItem iconItem) {
        mIconImage.LoadImageFile(0, iconItem.m_IconPath);
        mIconImage.SetColor(VPPMapMenu.VectorToARGB(iconItem.m_SelectedColor));
        mSelectedImage.Show(iconItem.m_IsSelected);
        if (iconItem.m_IsSelected) {
            mSelectedImage.LoadImageFile(0, "set:vpp_map_ui image:icon_frame");
        }
    }
}

class IconItem extends VPPMapItem {
    bool m_IsSelected = false;
    string m_IconPath;
    vector m_SelectedColor;
}