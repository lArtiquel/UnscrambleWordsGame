function Component()
{
}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
		// create shortcut
		if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/UnscrambleWordsGame.exe", "@StartMenuDir@/UnscrambleWordsGame.lnk",
            "workingDirectory=@TargetDir@");
		}
    } catch (e) {
        console.log(e);
    }
}

