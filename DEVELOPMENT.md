# Development

Cronedit is written in C++/C and without external dependencies. Supported platforms are debian and MacOS.

## How to develop locally

Clone the repository:

```bash
git clone git@github.com:codeclown/cronedit.git
cd cronedit/
```

Run tests:

```bash
make test
```

Build binaries (written into `bin/`):

```bash
make build
```

Run the build:

```bash
./bin/cronedit lib/sample.txt
```

## Release process

### CI builds

For each commit and pull request, GitHub Actions is used to automatically build binaries on:

- Ubuntu 18.04
- MacOS 10.15 (Catalina)

The resulting binaries are uploaded as artifacts into the respective GitHub workflow.

See [`.github/workflows/build.yml`](.github/workflows/build.yml).

### Making a release

1. Bump the version in [`lib/version.h`](lib/version.h), e.g. `#define CRONEDIT_VERSION "0.1.1"`
2. Add a git tag, e.g. `git tag v0.1.1`
3. Push with tags `git push --tags`
4. GitHub action detects the new tag and creates a draft release with attached binaries. Review and publish it here:
   https://github.com/codeclown/cronedit/releases
