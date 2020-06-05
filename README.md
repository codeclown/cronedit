# cronedit

> an interactive command line editor for crontab

## This project is in BETA!

**Note that this is an early version of cronedit. Versions 0.X.X should be considered beta versions and probably not suited for use in critical production environments. Recommended for use only in testing environments.**

If you want to be extra safe, or just play around, copy crontab contents into a file and edit that file with cronedit:

```bash
# copy crontab contents to .txt file
crontab -l > foobar.txt
# edit that file via cronedit
./cronedit foobar.txt
```

## Usage

```shell
$ cronedit --help
cronedit v0.1.0

Usage:
  ./bin/cronedit
  ./bin/cronedit <file>

Arguments:
  file            If given, this file is read and written to
                  instead of modifying crontab directly via
                  the 'crontab' command

Options:
  -h, --help      Show this help message
  -v, --version   Show cronedit version

Information about backups:
  Upon saving changes, previous crontab version is backed up
  into folder '$HOME/.cronedit'.

Source:
  This program is open source and available at:
  https://github.com/codeclown/cronedit

```

## Development

See [DEVELOPMENT.md](DEVELOPMENT.md).

## License

Released under 3-clause BSD License, see [LICENSE.md](LICENSE.md).
