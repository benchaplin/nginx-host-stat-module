/*
 * Copyright (C) 2023 by Ben Chaplin <benchaplin@protonmail.ch>
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_host_stat_init(ngx_conf_t *cf);
static ngx_int_t ngx_http_add_host_stat_header(ngx_http_request_t *r);

/**
 * This module provided directive: host_stat
 */
static ngx_command_t ngx_http_host_stat_commands[] = {
    { ngx_string("host_stat"), /* directive */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes no arguments*/
      ngx_http_add_host_stat_header, /* configuration setup function */
      0, /* No offset. Only one context is supported. */
      0, /* No offset when storing the module configuration on struct. */
      NULL},

    ngx_null_command /* command termination */
};

/* The module context. */
static ngx_http_module_t ngx_http_host_stat_module_ctx = {
    NULL, /* preconfiguration */
    ngx_http_host_stat_init, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

/* Module definition. */
ngx_module_t ngx_http_host_stat_module = {
    NGX_MODULE_V1,
    &ngx_http_host_stat_module_ctx, /* module context */
    ngx_http_host_stat_commands, /* module directives */
    NGX_HTTP_MODULE, /* module type */
    NULL, /* init master */
    NULL, /* init module */
    NULL, /* init process */
    NULL, /* init thread */
    NULL, /* exit thread */
    NULL, /* exit process */
    NULL, /* exit master */
    NGX_MODULE_V1_PADDING
};

/**
 * Add the Host-Stat header.
 *
 * @param r
 *   Pointer to the request structure. See http_request.h.
 * @return
 *   Status of the response generation.
 */
static char *
ngx_http_add_host_stat_header(ngx_http_request_t *r)
{
    ngx_table_elt_t *h = ngx_list_push(&r->headers_out.headers);
    if (h == NULL) {
        return NGX_CONF_ERROR;
    }
    h->key.data = (u_char *) "Host-Stat";
    h->key.len = 9;
    h->value.data = (u_char *) "hey";
    h->value.len = 3;

    return NGX_CONF_OK;
}

/**
 * Configuration setup function that sets up the header chain.
 *
 * @param cf
 *   Module configuration structure pointer.
 * @return
 *   Status of the configuration setup.
 */
static ngx_int_t 
ngx_http_host_stat_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_headers_filter;

    return NGX_OK;
}
