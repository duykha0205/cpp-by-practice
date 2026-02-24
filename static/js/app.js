// Theme toggle
(function() {
    const toggle = document.getElementById('theme-toggle');
    if (!toggle) return;

    const saved = localStorage.getItem('theme') || 'dark';
    document.documentElement.setAttribute('data-theme', saved);
    updateIcon(saved);

    toggle.addEventListener('click', function() {
        const current = document.documentElement.getAttribute('data-theme');
        const next = current === 'dark' ? 'light' : 'dark';
        document.documentElement.setAttribute('data-theme', next);
        localStorage.setItem('theme', next);
        updateIcon(next);
        updateHljsTheme(next);
    });

    function updateIcon(theme) {
        const icon = toggle.querySelector('.theme-icon');
        if (icon) {
            icon.textContent = theme === 'dark' ? '\u263E' : '\u2600';
        }
    }

    function updateHljsTheme(theme) {
        var dark = document.getElementById('hljs-dark');
        var light = document.getElementById('hljs-light');
        if (dark && light) {
            dark.disabled = (theme !== 'dark');
            light.disabled = (theme !== 'light');
        }
    }

    updateHljsTheme(saved);
})();

// Sidebar toggle
(function() {
    document.querySelectorAll('.sidebar-chapter-title').forEach(function(btn) {
        btn.addEventListener('click', function() {
            var exercises = this.nextElementSibling;
            if (exercises) {
                exercises.classList.toggle('open');
            }
        });
    });
})();

// Run code API
window.runCode = function(sourceCode, compilerFlags) {
    return fetch('/api/run', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
            source_code: sourceCode,
            compiler_flags: compilerFlags || ''
        })
    })
    .then(function(res) { return res.json(); });
};
