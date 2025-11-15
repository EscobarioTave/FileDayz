modded class MissionBase {
    override UIScriptedMenu CreateScriptedMenu(int id) {
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
        if (!menu) {
            switch (id) {
            case VPP_MENU_MAP:
                menu = new VPPMapMenu;
                break;
            }
            if (menu) {
                menu.SetID(id);
            }
        }
        return menu;
    }
}