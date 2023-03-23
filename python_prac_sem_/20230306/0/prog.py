import shlex
import readline
import cmd
import calendar

class CowShell(cmd.Cmd):
    def do_prmonth(self, arg):
        """Print a month’s calendar as returned by formatmonth()."""
        cur_args = parse(arg)
        y, m = cur_args[0], cur_args[1]
        calendar.prmonth(int(y), int(m))

    def complete_prmonth(self, pref, line, beg, end):
        try:
            cur = int(line.split()[-1])
        except:
            return ['2023', '2022', '2021', "your year", str(pref)]
        else:
            return ['1','2','3','4','5','6','7','8','9','10','11','12']
        

    def do_pryear(self, arg):
        """Print the calendar for an entire year as returned by formatyear()."""
        cur_args = parse(arg)
        y = int(cur_args[0])
        calendar.TextCalendar.pryear(y)


def parse(arg):
    return (tuple(shlex.split(arg)))


if __name__ == '__main__':
    CowShell().cmdloop()
