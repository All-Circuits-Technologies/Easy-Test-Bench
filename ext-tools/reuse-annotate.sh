#!/bin/sh

# SPDX-FileCopyrightText: 2023 Anthony Loiseau <anthony.loiseau@allcircuits.com>
# SPDX-FileCopyrightText: 2023 Benoit Rolandeau <benoit.rolandeau@allcircuits.com>
#
# SPDX-License-Identifier: LicenseRef-ALLCircuits-ACT-1.1

## This script simplifies the call of `reuse annotate` command, especially with
## the getting of git author name and emails. And also the usage of our ACT
## license: LicenseRef-ALLCircuits-ACT-X.Y.
## The license has to be contained in the LICENSES folder.
##
## By default the script only updates the staged files in git (the updated or
## added files): to prevent the adding of headers in temporary files.
##
## It requires reuse to be accessible from PATH.
##
## See REUSE website for more information: https://reuse.software
##
## USAGE: $0 [opts] [path]
##
## Options:
##  --reparse   Force the adding of the headers in all the files in the repo.
##              This is useful when the repo is an old one and no files have
##              SPDX info in their header.
##  --help      Show this help
##
## Path:
##  Alongside --reparse, optional path pattern to not repase all files

set -e
set -u
# shellcheck disable=SC3040
set -o pipefail


REPO_DIR="$(git rev-parse --show-toplevel)"
readonly REPO_DIR

# Should all files be reparsed from theit git history (0/1, defaults 0)
REPARSE=0

# Should specific file or folder be targetted (default to empty/none)
OPT_TARGET=

# The ACT license name which will be used in all our files
ACT_LICENSE_NAME="LicenseRef-ALLCircuits-ACT-1.1"

# The folder where the licenses are stored
LICENSES_FOLDER="${REPO_DIR}/LICENSES"

# The ACT license file path
ACT_LICENSE_FILE_PATH="${LICENSES_FOLDER}/${ACT_LICENSE_NAME}.txt"

# Variable holding a simple new line character, the POSIX way
NL="$(printf '\nx')"; NL="${NL%x}"
readonly NL

# Small helpers --------------------------------------------------------

die() {
    printf "%s" "$*" >&2
    exit 1
}

print_help() {
    grep "^##" "$0" | sed 's/^## \?//' | sed "s,\$0,$0,"
}

test_author_name_validity() {
    case ${1} in
        *\ *)
            # At least two tokens, OK
            true
            ;;

        *)
            false
            ;;
    esac
}

test_author_email_validity() {
    case ${1} in
        *@allcircuits.com)
            true
            ;;
        *@allcircuits-technologies.com)
            true
            ;;
        *@actech-ingenierie.fr)
            true
            ;;

        *)
            false
            ;;
    esac
}

# shellcheck disable=SC3043
get_copyright_info() {
    local author_name="${GIT_AUTHOR_NAME:-}"
    local author_email="${GIT_AUTHOR_EMAIL:-}"

    if test -z "${author_name}"
    then
        author_name="$(git config --default '' --get author.name)"

        if test -z "${author_name}"
        then
            author_name="$(git config --default '' --get user.name)"

            if test -z "${author_name}"
            then
                # shellcheck disable=SC1078,SC1079
                die "We can't get the author name from git, please fills your `
                    `git config file or use the env variable: GIT_AUTHOR_NAME"
            fi
        fi
    fi

    if test -z "${author_email}"
    then
        author_email="${EMAIL:-}"

        if test -z "${author_email}"
        then
            author_email="$(git config --default '' --get author.email)"

            if test -z "${author_email}"
            then
                author_email="$(git config --default '' --get user.email)"

                if test -z "${author_email}"
                then
                    # shellcheck disable=SC1078,SC1079
                    die "We can't get the author email from git, please fills `
                        `your git config file or use one of the env variables: `
                        `GIT_AUTHOR_EMAIL or EMAIL"
                fi
            fi
        fi
    fi

    if ! test_author_name_validity "${author_name}"
    then
        die "Bad author name '${author_name}'. Fix your git config file."
    elif ! test_author_email_validity "${author_email}"
    then
        die "Bad author email '${author_email}'. Fix your git config file."
    fi

    echo "${author_name} <${author_email}>"
}

test_license_presence() {
    if ! test -e "${ACT_LICENSE_FILE_PATH}"
    then
        # shellcheck disable=SC1078,SC1079
        die "The ACT license file is not stored in the expected place, add it `
            `in your repository here: ${ACT_LICENSE_FILE_PATH}"
    fi
}

# Tells if $1 is covered by dep5 (quick and dirty)
# Succeed if covered, fails if not covered.
# shellcheck disable=SC3043
test_path_in_dep5() {
    local path_to_test="$1"
    local dep5_file="${REPO_DIR}/.reuse/dep5"
    local covered="n"

    if test -f "${dep5_file}"
    then
        local dep5_covered_sub_pathes
        dep5_covered_sub_pathes="$(grep '^Files: ' "${dep5_file}" | sed 's/^Files: //' | sed 's/\*//')"


        for dep5_covered_sub_path in ${dep5_covered_sub_pathes}
        do
            if echo "${path_to_test}" | grep -qF "${dep5_covered_sub_path}"
            then
                covered="y"
                break;
            fi
        done
    fi

    test "${covered}" = "y"
}

# shellcheck disable=SC2215,SC3043
call_reuse() {
    test_license_presence

    copyright="$(get_copyright_info)"
    local copyright
    readonly copyright

    if test "${REPARSE}" -eq "0"
    then
        # Process staged files
        export IFS="${NL}"
        for git_file in $(git diff --name-only --cached --diff-filter=d)
        do
            if test_path_in_dep5 "${git_file}"
            then
                echo "Skipping ${git_file} (covered by dep5)"
                continue
            fi

            reuse annotate \
                --merge-copyrights \
                --license "${ACT_LICENSE_NAME}" \
                --copyright "${copyright}" \
                "${git_file}" || \
                \
                die "reuse crashed when working onto '${git_file}' with copyright '${copyright}'"
        done
    else
        # Reparse files from their git history
        export IFS="${NL}"
        for git_file in $(git ls-files "${OPT_TARGET:-.}")
        do
            if test_path_in_dep5 "${git_file}"
            then
                echo "Skipping ${git_file} (covered by dep5)"
                continue
            fi

            # Note: "uniq|sort|uniq" is an optimisation for "sort|uniq" when many consecutive items are identical
            local authors_name_email_year
            authors_name_email_year="$(git log --follow --format="%aN;%aE;%ad" --date="format:%Y" "${git_file}" | uniq | sort | uniq)"

            for name_email_year in ${authors_name_email_year}
            do
                local author_name
                local author_email
                local author_year

                author_name="${name_email_year%%;*}"
                author_email="$(printf "%s" "${name_email_year}" | cut -d ';' -f 2)"
                author_year="${name_email_year##*;}"

                if ! test_author_name_validity "${author_name}"
                then
                    die "Bad author name '${author_name}' for '${author_email}'. Need a 'Good Name <${author_email}>' row in your .mailmap"
                elif ! test_author_email_validity "${author_email}"
                then
                    die "Bad author email '${author_email}' for '${author_name}'. Need a '<good@corp.email> <${author_email}>' row in your .mailmap"
                fi

                # reuse
                reuse annotate \
                    --skip-unrecognised \
                    --merge-copyrights\
                    --license "${ACT_LICENSE_NAME}" \
                    --copyright "${author_name} <${author_email}>" \
                    --year "${author_year}" \
                    "${git_file}" || \
                    \
                    die "reuse crashed when working onto '${git_file}' with author '${author_name} <${author_email}>' (c) ${author_year}"
            done
        done
    fi
}

# Parse command line options and fire
# shellcheck disable=SC3043
main() {
    while test $# -gt 0
    do
        # Parse options
        local OPT="$1"
        case "${OPT}" in
        --reparse)
            REPARSE=1
            shift
            ;;
        --help)
            print_help
            exit 0
            ;;
        -*)
            die "Unknown option $1"
            ;;
        *)
            OPT_TARGET="${OPT}"
            shift
            ;;
        esac
    done

    call_reuse
}

main "$@"
