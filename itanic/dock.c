write_char('E');
write_char('n');
write_char('t');
write_char('e');
write_char('r');
write_char(' ');
write_char('D');
write_char('o');
write_char('c');
write_char('k');
write_char(' ');
write_char('N');
write_char('u');
write_char('m');
write_char('b');
write_char('e');
write_char('r');
write_char(':');
write_char(' ');

int dock[] = {'D','O','C','K','F','E','E','S','A','R','E','D','U','M','B', '\n'};

int flag = 1;
for (int ind = 0; , ind < 16, ind = ind + 1;)
{
    int tmp = read_char();
    if (tmp != dock[ind])
    {
        flag = 0;
    }
}
int endline = read_char();

if (flag == 1)
{
    write_char('X');
}
