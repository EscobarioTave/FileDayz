class VPPNewsElement
{
	string Title;
	string Content;
	ref array<ref VPPNewsButton> Buttons;

	void VPPNewsElement()
	{
		Buttons = new array<ref VPPNewsButton>;
	}

	array<ref VPPNewsButton> GetButtons()
	{
		return Buttons;
	}

	void AddButton(VPPNewsButton btn)
	{
		Buttons.Insert(btn);
	}
};