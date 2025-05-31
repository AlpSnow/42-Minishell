void	print_segment_list(t_segment *segment_list)
{
	t_word_fragments	*fragments;
	char				quote[4];

	printf("\033[1;34m========== Liste des segments ==========\033[0m\n");
	while (segment_list != NULL)
	{
		printf("\033[1;32mSegment #%d\033[0m  \033[1;32mType:\033[0m %d\n",
			segment_list->index, segment_list->type);
		printf("\033[1;36mFragments:\033[0m [");
		fragments = segment_list->content;
		while (fragments != NULL)
		{
			if (fragments->quote_type == QUOTE_SINGLE)
				printf("\033[31m'\033[0m%s\033[31m'\033[0m", fragments->str);
			else if (fragments->quote_type == QUOTE_DOUBLE)
				printf("\033[31m\"\033[0m%s\033[31m\"\033[0m", fragments->str);
			else
				printf("%s", fragments->str);
			if (fragments->next != NULL)
				printf(" \033[1;33m|\033[0m ");
			fragments = fragments->next;
		}
		printf("]\n");
		if (segment_list->are_there_quotes_inside)
			ft_strlcpy(quote, "Oui", sizeof(quote));
		else
			ft_strlcpy(quote, "Non", sizeof(quote));
		printf("\033[1;33mContient des quotes:\033[0m %s\n", quote);
		printf("\033[1;34m----------------------------------------\033[0m\n");
		segment_list = segment_list->next;
	}
}