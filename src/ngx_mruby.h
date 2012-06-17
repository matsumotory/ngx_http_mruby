#include <mruby.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <mruby/proc.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/string.h>

mrb_state   ngx_init_mruby(void);
void        ngx_free_mruby(mrb_state *mrb_interpreter);

mrb_parser_state    ngx_init_mruby_parser(char *filename, mrb_interpreter *interpreter);
void                ngx_free_mruby_parser(mrb_parser_state *parser);

ngx_int_t ngx_eval_mruby(mrb_state *mrb_interpreter, mrb_parser_state *parser, mrb_value *mrb_return_value, const ngx_log_t *log);
ngx_int_t ngx_parse_mruby(mrb_parser_state *parser, const ngx_log_t *log);

#include <ngx_mruby.c>
