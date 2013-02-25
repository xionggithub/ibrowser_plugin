/**********************************************************\

  Auto-generated ibrowserAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "ibrowser.h"

extern "C"{
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/installation_proxy.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/sbservices.h>
}

#ifndef H_ibrowserAPI
#define H_ibrowserAPI

#define F_SUCC  const boost::optional<FB::JSObjectPtr>& scb
#define F_ERRO  const boost::optional<FB::JSObjectPtr>& ecb
#define F_T_FLAG    boost::optional<bool> noThread
#define F_ADD F_SUCC,F_ERRO,F_T_FLAG
#define SUCC(a1,args...)    \
        if(scb && (*scb)->isValid()){ \
            (*scb)->InvokeAsync("", FB::variant_list_of( a1, ##args )); \
        }
#define ERRO(msg)   \
        if(ecb && (*ecb)->isValid()){ \
            (*ecb)->InvokeAsync("", FB::variant_list_of(msg)); \
        }
#define THREAD(fun,args...)                                         \
    do{                                                             \
        if(!noThread && scb && (*scb)->isValid() )                                               \
        {                                                           \
            /*printf("use thread:%s\n",__FUNCTION__);*/                 \
            boost::thread t(boost::bind(fun,this, ##args,scb,ecb,true));    \
            return true;                                            \
        }                                                           \
    }                                                               \
    while(0)

class ibrowserAPI;

class Call3back{
public:
    Call3back(
             FB::JSObjectPtr p,
             FB::JSObjectPtr s,
             FB::JSObjectPtr e){
        ibrowser=ib;
        fileName=fn;
        pcb=p;
        scb=s;
        ecb=e;
        
    }
    FB::JSObjectPtr pcb;
    FB::JSObjectPtr scb;
    FB::JSObjectPtr ecb;
};

class ibrowserAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn ibrowserAPI::ibrowserAPI(const ibrowserPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    ibrowserAPI(const ibrowserPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &ibrowserAPI::echo));
        registerMethod("testEvent", make_method(this, &ibrowserAPI::testEvent));
        
        registerMethod("init",      make_method(this, &ibrowserAPI::init));
        registerMethod("clean",      make_method(this, &ibrowserAPI::clean));
        registerMethod("getDeviceInfo", make_method(this, &ibrowserAPI::getDeviceInfo));
        registerMethod("getAppList",      make_method(this, &ibrowserAPI::getAppList));
        registerMethod("getSbservicesIconPngdata", make_method(this, &ibrowserAPI::getSbservicesIconPngdata));
        registerMethod("openDialog", make_method(this, &ibrowserAPI::openDialog));
        registerMethod("uploadFile", make_method(this, &ibrowserAPI::uploadFile));
        registerMethod("installPackage", make_method(this, &ibrowserAPI::installPackage));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &ibrowserAPI::get_testString,
                                       &ibrowserAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &ibrowserAPI::get_version));
        
        
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn ibrowserAPI::~ibrowserAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~ibrowserAPI() {};

    ibrowserPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    FB::variant init(F_ADD);
    FB::variant clean();
    FB::variant getDeviceInfo(const std::string& domain,F_ADD);
    FB::variant getAppList(F_ADD);
    FB::variant getSbservicesIconPngdata(const std::string& bundleId,F_ADD);
    FB::variant openDialog(F_ADD);
    FB::variant uploadFile(const std::string& fileName,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    FB::variant installPackage(const std::string& fileName,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    static void installCallback(const char *operation, plist_t status, void *user_data);
    void installPackageThread();
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
    void testEvent();

private:
    ibrowserWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;
    std::string uploadFileDir = "/Downloads";
    
    idevice_t device = NULL;
    instproxy_client_t instproxy_client = NULL;
    lockdownd_client_t 	lockdownd_client = NULL;
    sbservices_client_t sbservices_client = NULL;
    afc_client_t afc_client = NULL;
    
};

#endif // H_ibrowserAPI

