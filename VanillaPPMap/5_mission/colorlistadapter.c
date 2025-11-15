class ColorListAdapter extends ListAdapter {

    private EditDialog m_EditDialog;
    private vector m_SelectedColor;

    void ColorListAdapter(EditDialog dialog, Widget root) {
        super.init("VanillaPPMap/GUI/Layouts/List/HorizontalList.layout", root);

        m_EditDialog = dialog;
    }

    void SetSelectedColor(vector color) {
        m_SelectedColor = color;
        UpdateContent();
    }

    vector GetSelectedColor() {
        return m_SelectedColor;
    }

    void UpdateContent() {
        array<ref MarkerColor> colors = MarkerOptions.GetInstance().GetAllColors();
        array<ref VPPMapItem> items = new array<ref VPPMapItem>>;
        foreach (MarkerColor color : colors) {
            ColorItem colorItem = new ColorItem();
            colorItem.m_Color = color.getColorVector();
            if (!m_SelectedColor || m_SelectedColor == color.getColorVector()) {
                colorItem.m_IsSelected = true;
                m_SelectedColor = color.getColorVector();
            }
            items.Insert(colorItem);
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
        if (item.IsInherited(ColorItem)) {
            return ColorItemViewHolder(root);
        }
        return null;
    }

    override void FillViewHolder(ItemViewHolder viewHolder, VPPMapItem item) {
        if (item.IsInherited(ColorItem) && viewHolder.IsInherited(ColorItemViewHolder)) {
            ColorItemViewHolder vh = ColorItemViewHolder.Cast(viewHolder);
            ColorItem colorItem = ColorItem.Cast(item);
            vh.fillView(colorItem);
        }
    }

    override bool ShouldShowScrollBar() {
        float x, y;
        m_PanelListFrame.GetScreenSize( x, y );
        return m_ScrollWidget.GetContentWidth() > x;
    }

    override void OnItemClicked(string widgetName, VPPMapItem item) {

        if (widgetName == "color_image" || widgetName == "selected_image") {
            if (item.IsInherited(ColorItem)) {
                ColorItem colorItem = ColorItem.Cast(item);
                m_SelectedColor = colorItem.m_Color;
                m_EditDialog.OnColorSelected(m_SelectedColor);
                UpdateContent();
            }
        }
    }
}

class ColorItemViewHolder extends ItemViewHolder {
    ImageWidget mColorImage;
    ImageWidget mSelectedImage;

    void ColorItemViewHolder(Widget root) {
        super.init(GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/List/ColorListItem.layout", root));

        mColorImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "color_image" ) );
        mSelectedImage = ImageWidget.Cast( m_RootWidget.FindAnyWidget( "selected_image" ) );
    }

    void fillView(ColorItem colorItem) {
        mColorImage.SetColor(VPPMapMenu.VectorToARGB(colorItem.m_Color));
        mSelectedImage.Show(colorItem.m_IsSelected);
        if (colorItem.m_IsSelected) {
            mSelectedImage.LoadImageFile(0, "set:vpp_map_ui image:icon_frame");
        }
    }
}

class ColorItem extends VPPMapItem {
    bool m_IsSelected = false;
    vector m_Color = Vector(255, 255, 255);
}
