#include <ngx_config.h>
#include <ngx_http.h>
#include <ngx_conf_file.h>
#include <nginx.h>

typedef struct {
} nginx_http_mruby_loc_conf_t;

typedef struct {
} nginx_http_mruby_main_conf_t;

typedef struct {
} nginx_http_mruby_srv_conf_t;

static ngx_command_t ngx_http_mruby_commands[] = { };

static ngx_http_module_t ngx_http_mruby_module_ctx = {
    NULL,  /* preconfiguration */
    NULL,  /* postconfiguration */

    NULL,  /* create main configuration */
    NULL,  /* init main configuration */

    NULL,  /* create server configuration */
    NULL,  /* merge server configuration */

    NULL,  /* create location configuration */
    NULL   /* merge location configuration */
};

ngx_module_t ngx_http_mruby_module = {
    NGX_MODULE_V1,
    &ngx_http_mruby_module_ctx,     /* module context */
    ngx_http_mruby_commands,        /* module directives */
    NGX_HTTP_MODULE,                /* module type */
    NULL,                           /* init master */
    NULL,                           /* init module */
    NULL,                           /* init process */
    NULL,                           /* init thread */
    NULL,                           /* exit thread */
    NULL,                           /* exit process */
    NULL,                           /* exit master */
    NGX_MODULE_V1_PADDING
};
