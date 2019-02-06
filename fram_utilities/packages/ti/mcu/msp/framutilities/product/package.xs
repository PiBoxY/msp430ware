/*
 * ======== getLibs ========
 * This function is called during configuration to get the name of the
 * libraries that should be linked with.
 */
function getLibs()
{
    var FRAMUtilities = xdc.module(this.$name + ".FRAMUtilities");

    /* prepending '!' tells config to not fail if the lib doesn't yet exist */
    return ("!" + FRAMUtilities.getLibPath());
}
