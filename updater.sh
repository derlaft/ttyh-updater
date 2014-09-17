#!./busybox.exe

ARCH=$(cat arch)
BASEURL="https://ttyh.ru/files/$ARCH/"
get="./wget.exe --ca-certificate=ca.pem"

trap exit SIGINT


msg() {

    echo msg "$@"
}

err() {
    echo err "$@"
    exit 1
} 

get() {
    rm -f "$2"
    echo 'per 0'
    $get "$1" -O "$2" 2>&1 | awk '/%/ {print "per " $7}'
    echo 'per 100'
    [ -f "$2" ]
}

bad() {

    if ! sha1sum "$1" 2>/dev/null | grep -q -- "$2"; then
        msg "File $1 needs to be downloaded"
        return 0
    fi

    msg "File $1 is already at last revision"

    return 1
}

msg "Downloading file index"
get "$BASEURL/index" index.new || err 'Update check'

if [ ! -f index ] || ! cmp -s index index.new; then
    msg "Update required"
    cp index.new index
else
    msg "Update not required"
    exit 0
fi

cat index | while read hash file; do

    mkdir -p "$(dirname $file)"
    attempts=3

    while bad "$file" "$hash" && [[ $attempts -gt 0 ]]; do

        msg "Downloading $file"
        get "$BASEURL/$file.lzma" "$file.lzma"
        msg "Unpacking $file"
        lzma -df "$file.lzma"
        let attempts--
    done

    if bad "$file" "$hash"; then
        err "Downloading $file failed" >&2
        exit 1
    fi
done

msg "Update succeeded."
