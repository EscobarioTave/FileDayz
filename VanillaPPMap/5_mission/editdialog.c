class EditDialog extends ScriptedWidgetEventHandler {

    VPPMapMenu m_Menu;
    ref MarkerInfo m_Marker;
    int m_MarkerIndex;
    bool m_IsCustomServerMarker = false;
    string m_MarkerSuffix;

    Widget m_Root;
    EditBoxWidget m_EditBoxName;
    Widget m_EditBoxNameOverlay;
    Widget m_PanelColor;
    Widget m_PanelIcon;
    ButtonWidget m_ButtonDelete;
    ButtonWidget m_ButtonOK;
    Widget m_PanelClose;
    ButtonWidget m_ButtonClose;
    CheckBoxWidget m_CheckboxCustomServer;

    ref IconListAdapter m_IconListAdapter;
    ref ColorListAdapter m_ColorListAdapter;

    void EditDialog(VPPMapMenu menu, Widget root) {
        m_Menu = menu;
        m_Root = GetGame().GetWorkspace().CreateWidgets( "VanillaPPMap/GUI/Layouts/EditDialog.layout", root );
        m_EditBoxName = EditBoxWidget.Cast(m_Root.FindAnyWidget( "editbox_name" ));
        m_EditBoxNameOverlay = m_Root.FindAnyWidget( "editbox_name_overlay" );
        m_EditBoxNameOverlay.Show(false);
        m_PanelColor = m_Root.FindAnyWidget( "panel_color" );
        m_PanelIcon = m_Root.FindAnyWidget( "panel_icon" );
        m_ButtonDelete = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_delete" ));
        m_ButtonOK = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_ok" ));
        m_PanelClose = m_Root.FindAnyWidget( "panel_close" );
        m_ButtonClose = ButtonWidget.Cast(m_Root.FindAnyWidget( "button_close" ));
        m_CheckboxCustomServer = CheckBoxWidget.Cast(m_Root.FindAnyWidget( "checkbox_customserver" ));

        m_IconListAdapter = new IconListAdapter(m_PanelIcon);
        m_ColorListAdapter = new ColorListAdapter(this, m_PanelColor);

        m_Root.SetHandler(this);
    }

    void SetMarker(int markerIndex, MarkerInfo marker, bool isCustomServer, bool isNewMarker, string markerSuffix) {
        m_MarkerIndex = markerIndex;
        m_MarkerSuffix = markerSuffix;
        m_IsCustomServerMarker = isCustomServer;
        m_CheckboxCustomServer.SetChecked(m_IsCustomServerMarker);
        m_Marker = marker;
        m_EditBoxName.SetText(marker.GetName());
        m_EditBoxNameOverlay.Show(isNewMarker);
        m_IconListAdapter.SetSelectedIcon(marker.GetIconPath());
        m_IconListAdapter.SetSelectedColor(marker.GetColor());
        m_ColorListAdapter.SetSelectedColor(marker.GetColor());
    }

    void OnColorSelected(vector color) {
        m_IconListAdapter.SetSelectedColor(color);
    }

    override bool OnMouseButtonDown( Widget w, int x, int y, int button ) {
        if (button == MouseState.LEFT) {
            if (w == m_PanelClose || w == m_ButtonClose) {
                m_Menu.HideDialog();
                return true;
            } else if (w == m_ButtonDelete) {
                m_Menu.RemoveMarkerExact(m_Marker.GetPosition(), m_IsCustomServerMarker);
                m_Menu.HideDialog();
                return true;
            } else if (w == m_ButtonOK) {
                string name = m_EditBoxName.GetText();
                string iconPath = m_IconListAdapter.GetSelectedIcon();
                vector color = m_ColorListAdapter.GetSelectedColor();
                bool checked = m_CheckboxCustomServer.IsChecked();
                m_Menu.EditMarkerAttributes(m_MarkerIndex, name, m_MarkerSuffix, iconPath, color, m_IsCustomServerMarker, checked);
                m_Menu.HideDialog();
                return true;
            } else if (w == m_EditBoxNameOverlay) {
                m_EditBoxName.SetText("");
                m_EditBoxNameOverlay.Show(false);
                return false;
            }
        }
        return false;
    }
}