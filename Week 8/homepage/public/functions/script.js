function rewrite () {
    let page = window.location.pathname;
    let path = get_url(page);
    console.log(path);
    window.history.pushState("http://domain.ca", "Joshua", path );
}

function get_url (name) {
    if (name == '/index.html') {
        return '/home'
    }
    console.log(typeof(name));
    return name.slice(0, -5)
}