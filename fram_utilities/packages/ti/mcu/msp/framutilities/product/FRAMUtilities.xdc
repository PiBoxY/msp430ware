@Template("./FRAMUtilities.xdt")
metaonly module FRAMUtilities
{
    /*!
     *  ======== BASENAME ========
     *  The base name of the pre-built DriverLib library
     *  
     *  This name controls the name of a pre-built DriverLib library.  For
     *  example, if BASENAME is set to "driverlib" the name of the library
     *  will be driverlib.lib in the ./src/driverlib folder of your project.
     *  
     *  @_nodoc
     */
    const String BASENAME = "framutilities";

    /*!
     *  ======== getFamilyPath ========
     *  Given a device name, return the path to the device family's sources
     *
     *  This function returns a path relative to the DriverLib source
     *  repository containing the DriverLib sources.  For example,
     *  if `deviceName` is `"MSP430FR5969"`, this function returns
     *  `"driverlib/MSP430FR5xx_6xx"`.
     *
     *  The string returned by this function can be used with `getSrcRepo`
     *  to obtain the absolute path to the sources for the DriverLib family
     *  that supports the device `deviceName`.
     *  
     *  @see #getSrcRepo
     */
    String getFamilyPath(String deviceName);

    /*!
     *  ======== getLibPath ========
     *  Returns the path to a pre-built DriverLib library
     *  
     *  This function is called by the `getLibs()` method provided by the
     *  package containing the DriverLib module.  This `getLibs()` method
     *  is implemented in the `package.xs` file and is used to provide the
     *  path to a pre-built DriverLib library for any project that uses
     *  DriverLib.
     *  
     *  @_nodoc
     */
    String getLibPath();

    /*!
     *  ======== getSrcRepo ========
     *  Given a family path, return the path to it's source repository
     *
     *  This function returns an absolute path to the repository
     *  containing the DriverLib sources for the specified device family.
     *
     *  @see #getFamilyPath
     */
    String getSrcRepo(String familyPath);

    /*!
     *  ======== outputDir ========
     *  The output directory for all generated sources
     *
     *  The default output directory is the `src/` sub-directory of the
     *  directory containing the configuration script used to specify
     *  the peripherals for which to generate code.
     *
     *  This parameter should only be changed when running the
     *  configuration tool in a command line environment.  Integrated
     *  Development Environments (IDEs) often require that generated sources
     *  be placed in specific directories.  Changes to this parameter often
     *  require corresponding changes to the IDE.
     *
     *  @_nodoc
     */
    config String outputDir;
}
