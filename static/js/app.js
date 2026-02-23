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
    });

    function updateIcon(theme) {
        const icon = toggle.querySelector('.theme-icon');
        if (icon) {
            icon.textContent = theme === 'dark' ? '\u263E' : '\u2600';
        }
    }
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
