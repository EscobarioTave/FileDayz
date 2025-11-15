class VPPNewsButtonUI: ScriptedWidgetEventHandler
{
	private ButtonWidget  m_Root;
	private string        m_Url;
	private ImageWidget   vpp_btn_image;
	private TextWidget    vpp_btn_text;

	void VPPNewsButtonUI(WrapSpacerWidget root, VPPNewsButton data)
	{
		m_Url = data.Url;
		m_Root = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/NewsButton.layout", root));

		vpp_btn_image = ImageWidget.Cast(m_Root.FindAnyWidget("vpp_btn_image"));
		vpp_btn_text  = TextWidget.Cast(m_Root.FindAnyWidget("vpp_btn_text"));

		vpp_btn_text.SetText(data.Name);

		if (data.BtnIcon.Contains("set:") || data.BtnIcon.Contains("image:"))
			vpp_btn_image.LoadImageFile(0, data.BtnIcon);
			else
			vpp_btn_image.LoadImageFile(0, "VanillaPPMap\\GUI\\Textures\\" + data.BtnIcon + ".edds");

		m_Root.SetHandler(this);
	}

	void ~VPPNewsButtonUI()
	{
		if (m_Root != null)
			m_Root.Unlink();
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_Root)
		{
			GetGame().OpenURL(m_Url);
			return true;
		}
		return false;
	}
};

class VPPNewsElementUI: ScriptedWidgetEventHandler
{
	private ref Widget 		    m_Root;
	private MultilineTextWidget T1;
	private MultilineTextWidget C1;
	private WrapSpacerWidget    v_news_links; 
	private ref array<ref VPPNewsButtonUI> m_Buttons;

	void VPPNewsElementUI(GridSpacerWidget grid, VPPNewsElement data, bool makeDark = false)
	{
		m_Root 		 = GetGame().GetWorkspace().CreateWidgets("VanillaPPMap/GUI/Layouts/VPPNewsElement.layout", grid);
		T1     		 = MultilineTextWidget.Cast(m_Root.FindAnyWidget("T1"));
		C1     		 = MultilineTextWidget.Cast(m_Root.FindAnyWidget("C1"));
		v_news_links = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("v_news_links"));

		T1.SetText(data.Title);
		C1.SetText(data.Content);

		m_Buttons = new array<ref VPPNewsButtonUI>;
		array<ref VPPNewsButton> btns = data.GetButtons();
		foreach(VPPNewsButton b : btns)
		{
			m_Buttons.Insert(new VPPNewsButtonUI(v_news_links, b));
		}

		if (makeDark)
		{
			m_Root.SetAlpha(1.0);
		}

		m_Root.SetHandler(this);
	}

	void ~VPPNewsElementUI()
	{
		if (m_Root != null)
			m_Root.Unlink();
	}
};