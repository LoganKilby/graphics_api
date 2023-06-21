void skip_line() {
    
}

void parse_entity() {
    
}

void parse_resource() {
    
}


void skip_line(strparse_token_context *context) {
    while((context->parse_point < context->length) && (context->data[context->parse_point] != '\n')) context->parse_point++;
}


void read_scene_file(char *filename) {
    u32 size;
    char *file_data = (char *)os_read_entire_file(filename, &size);
    
    strparse_token_context token_context = {};
    token_context.data = file_data;
    token_context.length = size;
    token_context.parse_point = 0;
    
    unsigned int chars_read = 0;
    while((chars_read = strparse_next_token(&token_context, "", "")) != 0) {
        if(*token_context.token.string == '#') {
            skip_line(&token_context);
            continue;
        }
    }
    
    return;
}