/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-z63
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_mcu_msp_framutilities_product
{
    static final String VERS = "@(#) xdc-z63\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> icfgs = new ArrayList<Object>();
    ArrayList<String> inherits = new ArrayList<String>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.tools.product");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.mcu.msp.framutilities.product.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.mcu.msp.framutilities.product", new Value.Obj("ti.mcu.msp.framutilities.product", pkgP));
    }

    void ProductView$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.mcu.msp.framutilities.product.ProductView.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.mcu.msp.framutilities.product.ProductView", new Value.Obj("ti.mcu.msp.framutilities.product.ProductView", po));
        pkgV.bind("ProductView", vo);
        // decls 
        om.bind("ti.mcu.msp.framutilities.product.ProductView.ProductElemDesc", om.findStrict("xdc.tools.product.IProductView.ProductElemDesc", "ti.mcu.msp.framutilities.product"));
    }

    void FRAMUtilities$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities", new Value.Obj("ti.mcu.msp.framutilities.product.FRAMUtilities", po));
        pkgV.bind("FRAMUtilities", vo);
        // decls 
    }

    void ProductView$$CONSTS()
    {
        // module ProductView
    }

    void FRAMUtilities$$CONSTS()
    {
        // module FRAMUtilities
        om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities.BASENAME", "framutilities");
    }

    void ProductView$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void FRAMUtilities$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void ProductView$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void FRAMUtilities$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn FRAMUtilities.getFamilyPath
        fxn = (Proto.Fxn)om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$getFamilyPath", new Proto.Fxn(om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", "ti.mcu.msp.framutilities.product"), $$T_Str, 1, 1, false));
                fxn.addArg(0, "deviceName", $$T_Str, $$UNDEF);
        // fxn FRAMUtilities.getLibPath
        fxn = (Proto.Fxn)om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$getLibPath", new Proto.Fxn(om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", "ti.mcu.msp.framutilities.product"), $$T_Str, 0, 0, false));
        // fxn FRAMUtilities.getSrcRepo
        fxn = (Proto.Fxn)om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$getSrcRepo", new Proto.Fxn(om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", "ti.mcu.msp.framutilities.product"), $$T_Str, 1, 1, false));
                fxn.addArg(0, "familyPath", $$T_Str, $$UNDEF);
    }

    void ProductView$$SIZES()
    {
    }

    void FRAMUtilities$$SIZES()
    {
    }

    void ProductView$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/mcu/msp/framutilities/product/ProductView.xs");
        om.bind("ti.mcu.msp.framutilities.product.ProductView$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.mcu.msp.framutilities.product.ProductView.Module", "ti.mcu.msp.framutilities.product");
        po.init("ti.mcu.msp.framutilities.product.ProductView.Module", om.findStrict("xdc.tools.product.IProductView.Module", "ti.mcu.msp.framutilities.product"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("homeModule", $$T_Str, "ti.mcu.msp.framutilities.product.FRAMUtilities", "wh");
        po.addFld("linksToArray", new Proto.Arr($$T_Str, false), Global.newArray(new Object[]{"org.eclipse.rtsc.xdctools"}), "wh");
        po.addFld("driverLib", (Proto)om.findStrict("ti.mcu.msp.framutilities.product.ProductView.ProductElemDesc", "ti.mcu.msp.framutilities.product"), $$DEFAULT, "wh");
        po.addFld("product", (Proto)om.findStrict("ti.mcu.msp.framutilities.product.ProductView.ProductElemDesc", "ti.mcu.msp.framutilities.product"), $$DEFAULT, "wh");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.ProductView$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.ProductView$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.ProductView$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getProductDescriptor");
                if (fxn != null) po.addFxn("getProductDescriptor", (Proto.Fxn)om.findStrict("xdc.tools.product.IProductView$$getProductDescriptor", "ti.mcu.msp.framutilities.product"), fxn);
    }

    void FRAMUtilities$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/mcu/msp/framutilities/product/FRAMUtilities.xs");
        om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", "ti.mcu.msp.framutilities.product");
        po.init("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", $$Module);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("BASENAME", $$T_Str, "framutilities", "rh");
        po.addFld("outputDir", $$T_Str, $$UNDEF, "wh");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.mcu.msp.framutilities.product.FRAMUtilities$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                po.addFxn("getFamilyPath", (Proto.Fxn)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities$$getFamilyPath", "ti.mcu.msp.framutilities.product"), Global.get(cap, "getFamilyPath"));
                po.addFxn("getLibPath", (Proto.Fxn)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities$$getLibPath", "ti.mcu.msp.framutilities.product"), Global.get(cap, "getLibPath"));
                po.addFxn("getSrcRepo", (Proto.Fxn)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities$$getSrcRepo", "ti.mcu.msp.framutilities.product"), Global.get(cap, "getSrcRepo"));
    }

    void ProductView$$ROV()
    {
    }

    void FRAMUtilities$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.mcu.msp.framutilities.product.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.mcu.msp.framutilities.product"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/mcu/msp/framutilities/product/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.mcu.msp.framutilities.product"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.mcu.msp.framutilities.product"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.mcu.msp.framutilities.product"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.mcu.msp.framutilities.product"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.mcu.msp.framutilities.product"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.mcu.msp.framutilities.product"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.mcu.msp.framutilities.product", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.mcu.msp.framutilities.product");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.mcu.msp.framutilities.product.");
        pkgV.bind("$vers", Global.newArray("1, 0, 0"));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.mcu.msp.framutilities.product'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void ProductView$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.mcu.msp.framutilities.product.ProductView", "ti.mcu.msp.framutilities.product");
        po = (Proto.Obj)om.findStrict("ti.mcu.msp.framutilities.product.ProductView.Module", "ti.mcu.msp.framutilities.product");
        vo.init2(po, "ti.mcu.msp.framutilities.product.ProductView", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.mcu.msp.framutilities.product.ProductView$$capsule", "ti.mcu.msp.framutilities.product"));
        vo.bind("$package", om.findStrict("ti.mcu.msp.framutilities.product", "ti.mcu.msp.framutilities.product"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("ProductElemDesc", om.findStrict("xdc.tools.product.IProductView.ProductElemDesc", "ti.mcu.msp.framutilities.product"));
        tdefs.add(om.findStrict("xdc.tools.product.IProductView.ProductElemDesc", "ti.mcu.msp.framutilities.product"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.tools.product");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.mcu.msp.framutilities.product")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.mcu.msp.framutilities.product.ProductView$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("ProductView", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ProductView");
    }

    void FRAMUtilities$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities", "ti.mcu.msp.framutilities.product");
        po = (Proto.Obj)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities.Module", "ti.mcu.msp.framutilities.product");
        vo.init2(po, "ti.mcu.msp.framutilities.product.FRAMUtilities", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities$$capsule", "ti.mcu.msp.framutilities.product"));
        vo.bind("$package", om.findStrict("ti.mcu.msp.framutilities.product", "ti.mcu.msp.framutilities.product"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.mcu.msp.framutilities.product")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.mcu.msp.framutilities.product.FRAMUtilities$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", "./FRAMUtilities.xdt");
        atmap.seal("length");
        vo.bind("TEMPLATE$", "./FRAMUtilities.xdt");
        pkgV.bind("FRAMUtilities", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("FRAMUtilities");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.mcu.msp.framutilities.product.ProductView", "ti.mcu.msp.framutilities.product"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.mcu.msp.framutilities.product.FRAMUtilities", "ti.mcu.msp.framutilities.product"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.mcu.msp.framutilities.product.ProductView")).bless();
        ((Value.Obj)om.getv("ti.mcu.msp.framutilities.product.FRAMUtilities")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.mcu.msp.framutilities.product")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        ProductView$$OBJECTS();
        FRAMUtilities$$OBJECTS();
        ProductView$$CONSTS();
        FRAMUtilities$$CONSTS();
        ProductView$$CREATES();
        FRAMUtilities$$CREATES();
        ProductView$$FUNCTIONS();
        FRAMUtilities$$FUNCTIONS();
        ProductView$$SIZES();
        FRAMUtilities$$SIZES();
        ProductView$$TYPES();
        FRAMUtilities$$TYPES();
        if (isROV) {
            ProductView$$ROV();
            FRAMUtilities$$ROV();
        }//isROV
        $$SINGLETONS();
        ProductView$$SINGLETONS();
        FRAMUtilities$$SINGLETONS();
        $$INITIALIZATION();
    }
}
