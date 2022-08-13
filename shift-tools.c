void
shift(unsigned int *tag, int i)
{
	if (i > 0) /* left circular shift */
		*tag = ((*tag << i) | (*tag >> (LENGTH(tags) - i)));
	else       /* right circular shift */
		*tag = (*tag >> (- i) | *tag << (LENGTH(tags) + i));
}

/* send a window to the next/prev tag */
void
shifttag(const Arg *arg)
{
	Arg shifted = { .ui = selmon->tagset[selmon->seltags] };

	if (!selmon->clients)
		return;

	shift(&shifted.ui, arg->i);
	tag(&shifted);
}

/* view the next/prev tag */
void
shiftview(const Arg *arg)
{
	Arg shifted = { .ui = selmon->tagset[selmon->seltags] };

	shift(&shifted.ui, arg->i);
	view(&shifted);
}
