#pragma once
#include <WebServer.h>
#include "storage/config/NvsConfigProvider.h"

class SetupWebPortal
{
   public:
    explicit SetupWebPortal(NvsConfigProvider& nvs);

    void begin();
    void loop();  // call repeatedly

   private:
    static constexpr int MAX_COURSES = 8;

    NvsConfigProvider& nvs_;
    WebServer server_;

    void registerRoutes_();
    void handleIndex_();
    void handleSave_();
    void handleClear_();

    String renderIndexHtml_();
    String buildCourseRowsHtml_(const AppConfig* preload);

    static String htmlEscape_(const String& s);
};
