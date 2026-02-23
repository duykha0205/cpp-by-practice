// Initialize CodeMirror editors for exercise code blocks
(function() {
    var blocks = window.__CODE_BLOCKS__ || [];
    var editors = {};

    blocks.forEach(function(block) {
        var container = document.getElementById('editor-' + block.index);
        if (!container) return;

        if (typeof CodeMirrorFactory !== 'undefined') {
            editors[block.index] = CodeMirrorFactory.create(container, block.code);
        } else {
            // Fallback: plain textarea
            var textarea = document.createElement('textarea');
            textarea.className = 'editor-fallback';
            textarea.value = block.code;
            textarea.spellcheck = false;
            textarea.style.width = '100%';
            textarea.style.minHeight = '150px';
            textarea.style.padding = '1rem';
            textarea.style.fontFamily = 'var(--font-mono)';
            textarea.style.fontSize = '0.9rem';
            textarea.style.lineHeight = '1.5';
            textarea.style.background = 'var(--bg-editor)';
            textarea.style.color = 'var(--text-primary)';
            textarea.style.border = 'none';
            textarea.style.resize = 'vertical';
            textarea.style.outline = 'none';
            textarea.style.tabSize = '4';

            textarea.addEventListener('keydown', function(e) {
                if (e.key === 'Tab') {
                    e.preventDefault();
                    var start = this.selectionStart;
                    var end = this.selectionEnd;
                    this.value = this.value.substring(0, start) + '    ' + this.value.substring(end);
                    this.selectionStart = this.selectionEnd = start + 4;
                }
            });

            container.appendChild(textarea);
            editors[block.index] = {
                getValue: function() { return textarea.value; },
                setValue: function(v) { textarea.value = v; }
            };
        }
    });

    function getEditorValue(index) {
        var editor = editors[index];
        if (!editor) return '';
        return editor.getValue();
    }

    function setEditorValue(index, value) {
        var editor = editors[index];
        if (!editor) return;
        editor.setValue(value);
    }

    // Run buttons
    document.querySelectorAll('.run-btn').forEach(function(btn) {
        btn.addEventListener('click', function() {
            var index = parseInt(this.getAttribute('data-block-index'), 10);
            var code = getEditorValue(index);
            var outputPanel = document.getElementById('output-' + index);

            if (!code.trim()) return;

            btn.disabled = true;
            btn.classList.add('running');
            btn.textContent = 'Running...';

            outputPanel.className = 'output-panel visible';
            outputPanel.innerHTML = '<span style="color: var(--text-muted)">Compiling and running...</span>';

            window.runCode(code).then(function(result) {
                displayResult(outputPanel, result);
            }).catch(function(err) {
                outputPanel.className = 'output-panel visible error';
                outputPanel.innerHTML =
                    '<div class="output-header error">Error</div>' +
                    '<div class="output-body">' + escapeHtml(err.message) + '</div>';
            }).finally(function() {
                btn.disabled = false;
                btn.classList.remove('running');
                btn.innerHTML = '&#9654; Run';
            });
        });
    });

    // Reset buttons
    document.querySelectorAll('.reset-btn').forEach(function(btn) {
        btn.addEventListener('click', function() {
            var index = parseInt(this.getAttribute('data-block-index'), 10);
            var original = blocks.find(function(b) { return b.index === index; });
            if (original) {
                setEditorValue(index, original.code);
            }
            var outputPanel = document.getElementById('output-' + index);
            if (outputPanel) {
                outputPanel.className = 'output-panel';
                outputPanel.innerHTML = '';
            }
        });
    });

    function displayResult(panel, result) {
        var isSuccess = result.status === 'Success';

        if (result.status === 'Compilation Error') {
            panel.className = 'output-panel visible error';
            panel.innerHTML =
                '<div class="output-header error">Compilation Error</div>' +
                '<div class="output-body">' + escapeHtml(result.compile_output) + '</div>';
            return;
        }

        if (!isSuccess) {
            panel.className = 'output-panel visible error';
            var html = '<div class="output-header error">' + escapeHtml(result.status);
            if (result.exit_code > 0) {
                html += ' (exit code ' + result.exit_code + ')';
            }
            html += '</div>';
            if (result.stdout) {
                html += '<div class="output-body">' + escapeHtml(result.stdout) + '</div>';
            }
            if (result.stderr) {
                html += '<div class="output-body" style="color: var(--error)">' + escapeHtml(result.stderr) + '</div>';
            }
            panel.innerHTML = html;
            return;
        }

        panel.className = 'output-panel visible success';
        var html = '<div class="output-header success">Success</div>';
        html += '<div class="output-body">' + escapeHtml(result.stdout || '(no output)') + '</div>';
        if (result.stderr) {
            html += '<div class="output-body" style="color: var(--warning)">' + escapeHtml(result.stderr) + '</div>';
        }
        panel.innerHTML = html;
    }

    function escapeHtml(text) {
        if (!text) return '';
        var div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }
})();
