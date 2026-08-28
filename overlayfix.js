// Overlay Fix
// This module provides fixes and utilities for overlay management

export function initOverlay() {
  // Initialize overlay functionality
  console.log('Overlay initialized');
}

export function showOverlay(element) {
  if (element) {
    element.style.display = 'block';
  }
}

export function hideOverlay(element) {
  if (element) {
    element.style.display = 'none';
  }
}

export function toggleOverlay(element) {
  if (element) {
    const isHidden = element.style.display === 'none';
    element.style.display = isHidden ? 'block' : 'none';
  }
}

export default {
  initOverlay,
  showOverlay,
  hideOverlay,
  toggleOverlay
};
