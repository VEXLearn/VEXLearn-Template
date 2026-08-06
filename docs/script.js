/* VEXLearn Guide - Shared JavaScript */

// ========================
// Progress Bar
// ========================
function initProgressBar() {
  const bar = document.getElementById('progress-bar');
  if (!bar) return;
  window.addEventListener('scroll', () => {
    const scrollTop = window.scrollY;
    const docHeight = document.documentElement.scrollHeight - window.innerHeight;
    const pct = docHeight > 0 ? (scrollTop / docHeight) * 100 : 0;
    bar.style.width = pct + '%';
  });
}

// ========================
// Mobile Sidebar Toggle
// ========================
function initMobileNav() {
  const hamburger = document.querySelector('.nav-hamburger');
  const sidebar = document.querySelector('.sidebar');
  if (!hamburger || !sidebar) return;

  function setOpen(open) {
    sidebar.classList.toggle('open', open);
    hamburger.setAttribute('aria-expanded', String(open));
    hamburger.setAttribute('aria-label', open ? 'Close navigation' : 'Open navigation');
  }

  hamburger.addEventListener('click', () => {
    setOpen(!sidebar.classList.contains('open'));
  });

  // Close sidebar when clicking a link inside it
  sidebar.querySelectorAll('a').forEach(link => {
    link.addEventListener('click', () => {
      setOpen(false);
    });
  });

  // Close on outside click
  document.addEventListener('click', (e) => {
    if (!sidebar.contains(e.target) && !hamburger.contains(e.target)) {
      setOpen(false);
    }
  });

  document.addEventListener('keydown', (e) => {
    if (e.key === 'Escape' && sidebar.classList.contains('open')) {
      setOpen(false);
      hamburger.focus();
    }
  });
}

// ========================
// Active Nav Link
// ========================
function initActiveNav() {
  const currentPage = window.location.pathname.split('/').pop() || 'index.html';
  document.querySelectorAll('.nav-links a').forEach(a => {
    const href = a.getAttribute('href');
    if (href === currentPage || (currentPage === '' && href === 'index.html')) {
      a.classList.add('active');
      a.setAttribute('aria-current', 'page');
    }
  });
}

// ========================
// TOC Scroll Spy
// ========================
function initScrollSpy() {
  const tocLinks = document.querySelectorAll('.toc-list a');
  if (tocLinks.length === 0) return;

  const sections = Array.from(tocLinks)
    .map(a => {
      const id = a.getAttribute('href').replace('#', '');
      return document.getElementById(id);
    })
    .filter(Boolean);

  const observer = new IntersectionObserver(
    (entries) => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          tocLinks.forEach(a => a.classList.remove('active'));
          const active = document.querySelector(`.toc-list a[href="#${entry.target.id}"]`);
          if (active) active.classList.add('active');
        }
      });
    },
    { rootMargin: '-80px 0px -60% 0px' }
  );

  sections.forEach(s => observer.observe(s));
}

// ========================
// Copy Code Buttons
// ========================
function initCopyButtons() {
  document.querySelectorAll('.code-block').forEach(block => {
    const btn = block.querySelector('.copy-btn');
    const code = block.querySelector('code');
    if (!btn || !code) return;

    btn.setAttribute('type', 'button');
    btn.setAttribute('aria-live', 'polite');

    function showStatus(message, copied) {
      btn.textContent = message;
      btn.classList.toggle('copied', copied);
      setTimeout(() => {
        btn.textContent = 'Copy';
        btn.classList.remove('copied');
      }, 2000);
    }

    btn.addEventListener('click', () => {
      const text = code.innerText;
      const copy = navigator.clipboard ? navigator.clipboard.writeText(text) : Promise.reject();
      copy.then(() => {
        showStatus('Copied!', true);
      }).catch(() => {
        // Uses the older copy command only when the Clipboard API is unavailable.
        const ta = document.createElement('textarea');
        ta.value = text;
        ta.style.position = 'fixed';
        ta.style.opacity = '0';
        document.body.appendChild(ta);
        ta.select();
        const copied = document.execCommand('copy');
        document.body.removeChild(ta);
        showStatus(copied ? 'Copied!' : 'Copy failed', copied);
      });
    });
  });
}

// ========================
// Tab System
// ========================
function initTabs() {
  document.querySelectorAll('.tabs').forEach(tabGroup => {
    const tabList = tabGroup.querySelector('.tab-buttons');
    const buttons = Array.from(tabGroup.querySelectorAll('.tab-btn'));
    const contents = Array.from(tabGroup.querySelectorAll('.tab-content'));
    if (!tabList || buttons.length === 0) return;

    tabList.setAttribute('role', 'tablist');

    function activateTab(index, moveFocus = false) {
      buttons.forEach((button, i) => {
        const active = i === index;
        button.classList.toggle('active', active);
        button.setAttribute('aria-selected', String(active));
        button.tabIndex = active ? 0 : -1;
        contents[i].classList.toggle('active', active);
        contents[i].hidden = !active;
      });
      if (moveFocus) buttons[index].focus();
    }

    buttons.forEach((btn, i) => {
      if (!contents[i]) return;
      const tabId = `tab-${tabGroup.dataset.tabs || 'group'}-${i}`;
      const panelId = contents[i].id || `${tabId}-panel`;
      btn.id = tabId;
      btn.setAttribute('type', 'button');
      btn.setAttribute('role', 'tab');
      btn.setAttribute('aria-controls', panelId);
      contents[i].id = panelId;
      contents[i].setAttribute('role', 'tabpanel');
      contents[i].setAttribute('aria-labelledby', tabId);

      btn.addEventListener('click', () => activateTab(i));
      btn.addEventListener('keydown', (e) => {
        let next = i;
        if (e.key === 'ArrowRight') next = (i + 1) % buttons.length;
        else if (e.key === 'ArrowLeft') next = (i - 1 + buttons.length) % buttons.length;
        else if (e.key === 'Home') next = 0;
        else if (e.key === 'End') next = buttons.length - 1;
        else return;
        e.preventDefault();
        activateTab(next, true);
      });
    });

    const initial = Math.max(0, buttons.findIndex(button => button.classList.contains('active')));
    activateTab(initial);
  });
}

// ========================
// Interactive Checklist
// ========================
function initChecklists() {
  document.querySelectorAll('.checklist').forEach(list => {
    const items = list.querySelectorAll('li');
    const progressEl = list.previousElementSibling;
    const storageKey = list.dataset.storageKey;

    if (storageKey) {
      try {
        const completed = JSON.parse(localStorage.getItem(storageKey) || '[]');
        items.forEach(item => item.classList.toggle('done', completed.includes(item.dataset.checkId)));
      } catch (error) {
        // Progress still works for this visit when browser storage is unavailable or invalid.
      }
    }

    function updateProgress() {
      const done = list.querySelectorAll('li.done').length;
      const total = items.length;
      if (progressEl && progressEl.classList.contains('checklist-progress')) {
        const fill = progressEl.querySelector('.checklist-progress-fill');
        const text = progressEl.querySelector('.checklist-progress-text');
        if (fill) fill.style.width = (done / total * 100) + '%';
        if (text) text.textContent = `${done} / ${total} done`;
        progressEl.setAttribute('role', 'progressbar');
        progressEl.setAttribute('aria-valuemin', '0');
        progressEl.setAttribute('aria-valuemax', String(total));
        progressEl.setAttribute('aria-valuenow', String(done));
      }
    }

    items.forEach(item => {
      item.tabIndex = 0;
      item.setAttribute('role', 'checkbox');
      item.setAttribute('aria-checked', String(item.classList.contains('done')));

      function toggleItem() {
        item.classList.toggle('done');
        item.setAttribute('aria-checked', String(item.classList.contains('done')));
        if (storageKey) {
          try {
            const completed = [...items]
              .filter(checkItem => checkItem.classList.contains('done'))
              .map(checkItem => checkItem.dataset.checkId);
            localStorage.setItem(storageKey, JSON.stringify(completed));
          } catch (error) {
            // A blocked or full storage area must not disable the checklist.
          }
        }
        updateProgress();
      }

      item.addEventListener('click', toggleItem);
      item.addEventListener('keydown', (e) => {
        if (e.key === ' ' || e.key === 'Enter') {
          e.preventDefault();
          toggleItem();
        }
      });
    });

    updateProgress();
  });
}

// ========================
// Back to Top Button
// ========================
function initBackToTop() {
  const reduceMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;
  document.querySelectorAll('.back-to-top').forEach(btn => {
    btn.addEventListener('click', () => {
      window.scrollTo({ top: 0, behavior: reduceMotion ? 'auto' : 'smooth' });
    });
  });
}

// ========================
// PID Visualizer
// ========================
function initPIDVisualizer() {
  const canvas = document.getElementById('pid-canvas');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');

  const kpInput = document.getElementById('pid-kp');
  const kiInput = document.getElementById('pid-ki');
  const kdInput = document.getElementById('pid-kd');
  const kpNum  = document.getElementById('pid-kp-num');
  const kiNum  = document.getElementById('pid-ki-num');
  const kdNum  = document.getElementById('pid-kd-num');
  const kpVal = document.getElementById('pid-kp-val');
  const kiVal = document.getElementById('pid-ki-val');
  const kdVal = document.getElementById('pid-kd-val');
  const runBtn = document.getElementById('pid-run');
  const status = document.getElementById('pid-status');
  const reduceMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches;

  let animFrame = null;

  function resizeCanvas() {
    const rect = canvas.parentElement.getBoundingClientRect();
    canvas.width = rect.width - 48;
    canvas.height = 200;
  }

  function simulate(Kp, Ki, Kd) {
    const steps = 150;
    const target = 1.0;
    const dt = 0.1;
    let pos = 0, vel = 0, integral = 0, prevError = 0;
    const positions = [0];

    for (let i = 0; i < steps; i++) {
      const error = target - pos;
      integral += error * dt;
      if (Math.abs(integral) > 2) integral = Math.sign(integral) * 2;
      const derivative = (error - prevError) / dt;
      const force = Kp * error + Ki * integral + Kd * derivative;
      vel += force * dt * 0.5;
      vel *= 0.92;
      pos += vel * dt;
      prevError = error;
      positions.push(pos);
    }
    return positions;
  }

  function draw() {
    const Kp = parseFloat(kpInput.value);
    const Ki = parseFloat(kiInput.value);
    const Kd = parseFloat(kdInput.value);

    if (kpVal) kpVal.textContent = Kp.toFixed(1);
    if (kiVal) kiVal.textContent = Ki.toFixed(2);
    if (kdVal) kdVal.textContent = Kd.toFixed(1);

    const data = simulate(Kp, Ki, Kd);
    const w = canvas.width;
    const h = canvas.height;
    const pad = 38;

    const maxVal = 1.8;
    const minVal = -0.5;
    const range = maxVal - minVal;
    function toX(i) { return pad + (i / (data.length - 1)) * (w - pad - 16); }
    function toY(v) { return h - pad - ((v - minVal) / range) * (h - pad - 16); }

    ctx.clearRect(0, 0, w, h);

    // Background grid
    ctx.strokeStyle = 'rgba(42,42,80,0.8)';
    ctx.lineWidth = 1;
    for (let i = 0; i <= 4; i++) {
      const y = 16 + (h - pad - 16) * (i / 4);
      ctx.beginPath();
      ctx.moveTo(pad, y);
      ctx.lineTo(w - 16, y);
      ctx.stroke();
    }

    // Target line placed at value 1.0
    const targetY = toY(1.0);
    ctx.strokeStyle = 'rgba(245, 166, 35, 0.5)';
    ctx.lineWidth = 1;
    ctx.setLineDash([6, 4]);
    ctx.beginPath();
    ctx.moveTo(pad, targetY);
    ctx.lineTo(w - 16, targetY);
    ctx.stroke();
    ctx.setLineDash([]);
    ctx.fillStyle = 'rgba(245, 166, 35, 0.7)';
    ctx.font = '11px monospace';
    ctx.fillText('Target', w - 16 - 46, targetY - 5);

    // Plot curve
    ctx.strokeStyle = '#7b3fe4';
    ctx.lineWidth = 2.5;
    ctx.shadowColor = 'rgba(123,63,228,0.6)';
    ctx.shadowBlur = 6;
    ctx.beginPath();
    data.forEach((v, i) => {
      if (i === 0) ctx.moveTo(toX(i), toY(v));
      else ctx.lineTo(toX(i), toY(v));
    });
    ctx.stroke();
    ctx.shadowBlur = 0;

    // Y-axis tick labels
    ctx.fillStyle = 'rgba(144,144,176,0.8)';
    ctx.font = '10px monospace';
    ctx.textAlign = 'right';
    [0, 0.5, 1.0].forEach(v => {
      ctx.fillText(v.toFixed(1), pad - 5, toY(v) + 4);
    });
    ctx.textAlign = 'left';

    // Y-axis title (rotated)
    ctx.save();
    ctx.translate(10, h / 2 - 8);
    ctx.rotate(-Math.PI / 2);
    ctx.fillStyle = 'rgba(144,144,176,0.6)';
    ctx.font = '10px monospace';
    ctx.textAlign = 'center';
    ctx.fillText('Position (units)', 0, 0);
    ctx.restore();

    // X-axis title
    ctx.fillStyle = 'rgba(144,144,176,0.6)';
    ctx.font = '10px monospace';
    ctx.textAlign = 'center';
    ctx.fillText('Time (steps)', w / 2, h - 4);
    ctx.textAlign = 'left';

    // Overshoot indicator
    const maxPos = Math.max(...data);
    if (maxPos > 1.02) {
      const pct = ((maxPos - 1) * 100).toFixed(1);
      ctx.fillStyle = 'rgba(231,76,60,0.8)';
      ctx.font = '11px monospace';
      ctx.fillText(`Overshoot: ${pct}%`, pad + 8, 30);
    }

    // Final error
    const finalPos = data[data.length - 1];
    const err = Math.abs(finalPos - 1);
    ctx.fillStyle = err < 0.05 ? 'rgba(46,204,113,0.8)' : 'rgba(245,166,35,0.8)';
    ctx.font = '11px monospace';
    ctx.fillText(`Final error: ${(err * 100).toFixed(1)}%`, w - 16 - 140, 30);
    if (status) status.textContent = `PID simulation final error: ${(err * 100).toFixed(1)} percent.`;
  }

  resizeCanvas();
  draw();

  // Sync range sliders <-> number inputs bidirectionally
  [[kpInput, kpNum, 1], [kiInput, kiNum, 2], [kdInput, kdNum, 1]].forEach(([range, num, dec]) => {
    if (range) range.addEventListener('input', () => {
      if (num) num.value = parseFloat(range.value).toFixed(dec);
      draw();
    });
    if (num) num.addEventListener('input', () => {
      let v = parseFloat(num.value);
      if (isNaN(v)) return;
      v = Math.min(parseFloat(range.max), Math.max(parseFloat(range.min), v));
      range.value = v;
      draw();
    });
  });

  if (runBtn) {
    runBtn.addEventListener('click', () => {
      if (reduceMotion) {
        draw();
        if (status) status.textContent += ' Animation is disabled by your reduced-motion preference.';
        return;
      }
      if (animFrame) cancelAnimationFrame(animFrame);
      const Kp = parseFloat(kpInput.value);
      const Ki = parseFloat(kiInput.value);
      const Kd = parseFloat(kdInput.value);
      const anim = simulate(Kp, Ki, Kd);
      let step = 0;

      function frame() {
        const Kp2 = parseFloat(kpInput.value);
        const Ki2 = parseFloat(kiInput.value);
        const Kd2 = parseFloat(kdInput.value);
        const partial = simulate(Kp2, Ki2, Kd2).slice(0, step + 1);

        const w = canvas.width, h = canvas.height, pad = 38;
        const maxVal = 1.8, minVal = -0.5, range = maxVal - minVal;
        function toX(i) { return pad + (i / (anim.length - 1)) * (w - pad - 16); }
        function toY(v) { return h - pad - ((v - minVal) / range) * (h - pad - 16); }

        ctx.clearRect(0, 0, w, h);

        ctx.strokeStyle = 'rgba(42,42,80,0.8)';
        ctx.lineWidth = 1;
        for (let g = 0; g <= 4; g++) {
          const y = 16 + (h - pad - 16) * (g / 4);
          ctx.beginPath(); ctx.moveTo(pad, y); ctx.lineTo(w - 16, y); ctx.stroke();
        }

        const tY = toY(1.0);
        ctx.strokeStyle = 'rgba(245,166,35,0.5)'; ctx.lineWidth = 1; ctx.setLineDash([6,4]);
        ctx.beginPath(); ctx.moveTo(pad, tY); ctx.lineTo(w - 16, tY); ctx.stroke();
        ctx.setLineDash([]);
        ctx.fillStyle = 'rgba(245,166,35,0.7)'; ctx.font = '11px monospace';
        ctx.fillText('Target', w - 16 - 46, tY - 5);

        ctx.fillStyle = 'rgba(144,144,176,0.8)'; ctx.font = '10px monospace';
        ctx.textAlign = 'right';
        [0, 0.5, 1.0].forEach(v => { ctx.fillText(v.toFixed(1), pad - 5, toY(v) + 4); });
        ctx.textAlign = 'left';
        ctx.save(); ctx.translate(10, h / 2 - 8); ctx.rotate(-Math.PI / 2);
        ctx.fillStyle = 'rgba(144,144,176,0.6)'; ctx.font = '10px monospace';
        ctx.textAlign = 'center'; ctx.fillText('Position (units)', 0, 0); ctx.restore();
        ctx.fillStyle = 'rgba(144,144,176,0.6)'; ctx.font = '10px monospace';
        ctx.textAlign = 'center'; ctx.fillText('Time (steps)', w / 2, h - 4); ctx.textAlign = 'left';

        ctx.strokeStyle = '#7b3fe4'; ctx.lineWidth = 2.5;
        ctx.shadowColor = 'rgba(123,63,228,0.6)'; ctx.shadowBlur = 6;
        ctx.beginPath();
        partial.forEach((v, idx) => {
          if (idx === 0) ctx.moveTo(toX(idx), toY(v));
          else ctx.lineTo(toX(idx), toY(v));
        });
        ctx.stroke(); ctx.shadowBlur = 0;

        step++;
        if (step < anim.length) animFrame = requestAnimationFrame(frame);
      }

      animFrame = requestAnimationFrame(frame);
    });
  }

  window.addEventListener('resize', () => { resizeCanvas(); draw(); });
}

// ========================
// highlight.js init
// ========================
function initHighlight() {
  if (typeof hljs !== 'undefined') {
    document.querySelectorAll('pre code').forEach(block => {
      hljs.highlightElement(block);
    });
  }
}

// ========================
// Init All
// ========================
document.addEventListener('DOMContentLoaded', () => {
  initProgressBar();
  initMobileNav();
  initActiveNav();
  initScrollSpy();
  initCopyButtons();
  initTabs();
  initChecklists();
  initBackToTop();
  initPIDVisualizer();
  initHighlight();
});
