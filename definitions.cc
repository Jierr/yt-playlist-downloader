#include "definitions.hh"

xml_ctx_t xctx;
track_t xtrack;
Playlist xpl;

void init_stack(tag_stack_t* stack)
{
	stack->count = 0;
	stack->top = 0;
}

void clean_stack(tag_stack_t* stack)
{
	tag_stack_elem_t* next = 0;
	stack->count = 0;
	while(stack->top)
	{
		next = stack->top->prev;
		free(stack->top->name);
		free(stack->top);
		stack->top = next;
	}
	stack->top = 0;
}

tag_stack_elem_t* pop_stack(tag_stack_t* stack)
{
	tag_stack_elem_t* result = 0;
	if(!stack->top)
		return 0;

	result = stack->top;
	stack->top = stack->top->prev;
	--stack->count;

	return result;
}

int close_valid(tag_stack_t* stack, char* name)
{
	tag_stack_elem_t* result = 0;
	int valid = 0;
	result = pop_stack(stack);
	if(strcmp(result->name, name) == 0)
		valid = 1;

	free(result->name);
	free(result);

	return valid;
}

int push_stack(tag_stack_t* stack, char* name)
{
	int len = 0;
	tag_stack_elem_t* elem = 0;

	elem = (tag_stack_elem_t*)malloc(sizeof(tag_stack_elem_t));
	++stack->count;
	elem->prev = stack->top;
	len = strlen(name);
	elem->name = (char*)malloc(sizeof(char)*(len+1));
	strcpy(elem->name, name);
	stack->top = elem;
	return stack->count;
}

void free_stack_elem(tag_stack_elem_t* elem)
{
	free(elem->name);
	free(elem);
}

void init_xml_ctx(xml_ctx_t* ctx)
{
	ctx->curr_str = "";
	ctx->line = 0;
	init_stack(&ctx->stack);
}

void reset_xml_ctx(xml_ctx_t* ctx)
{
	ctx->curr_str = "";
	ctx->line = 0;
	clean_stack(&ctx->stack);
}


int str_add(char** dst, char* add)
{
	char* result = 0;
	int alen = 0;
	int dlen = 0;

	if(!dst)
		return 0;

	if(add)
		alen = strlen(add);

	if(*dst)
		dlen = strlen(*dst);

	if(!add)
		return 0;

	if(alen+dlen > 0)
	{
		result = (char*)malloc((alen+dlen+1)*sizeof(char));
		if(*dst)
			strcpy(result, *dst);
		strcpy(&result[dlen], add);
		result[dlen+alen] = 0;
	}
	if(*dst)
		free(*dst);

	(*dst) = result;

	return alen + dlen;
}

void construct_valid_filename(string& name)
{
    int c = 0;
    int v = 0;
    int invalid = 0;
    const char file_invalid[] = "\\/:*?\"<>|";

    while(name[c])
    {
        v = 0;
        invalid = 0;
        while(file_invalid[v] && !invalid)
        {
            if(name[c] == file_invalid[v])
            {
                name[c] = '-';
                invalid = 1;
            }
            ++v;
        }
        ++c;
    }
}
