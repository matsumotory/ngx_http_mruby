mrb_state ngx_init_mruby(void) {
    return mrb_open();
}

mrb_state ngx_free_mruby(mrb_state *mrb_interpreter) {
    mrb_close(mrb_interpreter);
}

mrb_parser_state ngx_init_mruby_parser(ngx_str_t *source_filename, const ngx_log_t *log) {
    FILE            *source_file;
    char            *source;
    struct stat     source_file_stat;
    unsigned int    source_size; 

    struct mrb_parser_state *parser;

    if (source_filename == NULL) {
        ngx_log_error(NGX_LOG_CRIT, log, "mruby source filename is invalid");
        return NULL;
    }

    mrb_interpreter = mrb_open();

    if (mrb_interpreter == NULL) {
        ngx_log_error(NGX_LOG_CRIT, "could not allocate mruby interpreter");
        return NULL;
    }

    parser = mrb_parser_new(mrb_interpreter);

    stat(source_filename, &source_file_stat);

    if (source_file_stat == NULL) {
        ngx_log_error(NGX_LOG_CRIT, log, "mruby file '%s' does not exist.", source_filename);
        return NULL;
    }

    source_size = (source_file_stat.st_size * sizeof(char)) + 1;
    source      = calloc(source_size);

    if (source == NULL) {
        ngx_log_error(NGX_LOG_CRIT, log, "could not allocate memory for mruby source");
        return NULL;
    }

    source_file = fopen(source_filename, "r");
    fread(source, sizeof(char), source_file_stat.st_size, source_file);

    if (ferror(source_file)) {
        ngx_log_error(
            NGX_LOG_CRIT, 
            log, 
            "could not read mruby file '%s': %s", 
            source_filename, 
            ngx_strerror(ferror(source_file))
        );

        free(source);

        return NULL;
    } else {
        fclose(source_file);
    }

    parser->filename        = (char *)source_filename;
    parser->s               = source;
    parser->send            = source + strlen(source);
    parser->capture_errors  = 1;
    parser->lineno          = 1;

    return parser;
}

ngx_int_t ngx_parse_mruby(mrb_parser_state *parser, const ngx_log_t *log) {
    mrb_parser_parse(parser, NULL);

    if (parser->nerr > 0) {
        int i = parser->nerr;
        int x = 0;

        while (++x <= i) {
            ngx_log_error( 
                NGX_LOG_CRIT,
                log
                "file %s, line %d: %s\n", 
                parser->filename,
                parser->error_buffer[x-1].lineno, 
                parser->error_buffer[x-1].message
            );
        }

        return 1;
    }

    return 0;
}

ngx_int_t ngx_eval_mruby(mrb_state *mrb_interpreter, mrb_parser_state *parser, mrb_value *mrb_return_value, const ngx_log_t *log) {
    *mrb_return_value = 
        mrb_run(
            mrb_interpreter,
            mrb_proc_new(
                mrb_interpreter,
                mrb_interpreter->irep[
                    mrb_generate_code(mrb_interpreter, parser->tree)
                ]
                ),
            mrb_top_self(mrb_interpreter)
        );

    /* exception handling */
    if (mrb_interpreter->exc) {
        mrb_p(mrb_interpreter, mrb_obj_value(mrb_interpreter->exc));
        mrb_interpreter->exc = NULL;
        return 1;
    }

    return 0;
}

void ngx_free_mruby_parser(mrb_parser_state *parser) {
    // do we need to free the filename here too?
    free(parser->source);
    free(parser);
}
