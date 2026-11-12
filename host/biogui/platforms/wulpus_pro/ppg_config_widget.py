# Copyright University of Bologna - ETH Zurich 2026
# Licensed under Apache v2.0 see LICENSE for details.
#
# SPDX-License-Identifier: Apache-2.0

"""
PPG (MAXM86161) CONFIGURATION WIDGET, BUILT LIKE WulpusConfigWidget.
THE 11 SETTINGS FOLLOW ppg_config_t IN THE FIRMWARE (ppg_new_appl.h).
"""

from __future__ import annotations

import json
import logging
from dataclasses import asdict, dataclass
from pathlib import Path

from PySide6.QtGui import QIntValidator
from PySide6.QtWidgets import (
    QCheckBox,
    QComboBox,
    QFileDialog,
    QFormLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMessageBox,
    QPushButton,
    QVBoxLayout,
    QWidget,
)

logger = logging.getLogger(__name__)

# COMMAND CODE OF START_PPG_STREAMING IN commands.c
PPG_START_CMD = 39

# INDEX -> TEXT, SAME ORDER AS THE FIRMWARE EXPECTS (0, 1, 2, 3, ...)
LED_RANGE_MA = [31, 62, 93, 124]
TINT_US = [14.8, 29.4, 58.7, 117.3]
ADC_RANGE_UA = [4, 8, 16, 32]
SAMPLE_AVG = [1, 2, 4, 8, 16, 32, 64, 128]


@dataclass
class PpgConfig:
    """ONE PPG CONFIGURATION, FIELD ORDER = BYTE ORDER OF THE START COMMAND."""

    sensor_mask: int = 0x01
    sample_rate_hz: int = 125
    led_green: int = 0x7F
    led_ir: int = 0x7F
    led_red: int = 0x7F
    led_range: int = 3
    tint: int = 3
    adc_range: int = 3
    sample_avg: int = 0
    alc_enable: int = 1
    proximity_enable: int = 0

    def get_start_package(self) -> bytes:
        """12 BYTES: COMMAND 39 + THE 11 SETTINGS."""
        return bytes([PPG_START_CMD, *asdict(self).values()])

    @property
    def packet_rate_hz(self) -> float:
        """THE FIRMWARE PACKS 4 SAMPLES PER PACKET."""
        return self.sample_rate_hz / 4


class PpgConfigWidget(QWidget):
    """CONFIGURATION WIDGET FOR THE MAXM86161 PPG SENSOR."""

    def __init__(self, parent: QWidget | None = None) -> None:
        super().__init__(parent)

        self._presets_dir = self._get_presets_directory()
        self._loading_preset = False

        self._build_ui()
        self._setup_validators()
        self._populate_combo_boxes()
        self._populate_presets()
        self._connect_signals()

        self.load_config(PpgConfig())
        logger.info("PpgConfigWidget initialized")

    # ── UI ────────────────────────────────────────────────────────────────
    def _build_ui(self) -> None:
        main_layout = QVBoxLayout(self)

        # PRESET ROW, SAME AS IN WulpusConfigWidget
        preset_box = QGroupBox("Preset", self)
        preset_layout = QHBoxLayout(preset_box)
        self.presetComboBox = QComboBox(preset_box)
        self.saveConfigButton = QPushButton("Save as preset...", preset_box)
        preset_layout.addWidget(self.presetComboBox, 1)
        preset_layout.addWidget(self.saveConfigButton)
        main_layout.addWidget(preset_box)

        form_box = QGroupBox("MAXM86161", self)
        self.formLayout = QFormLayout(form_box)

        self.sampleRateLineEdit = QLineEdit(form_box)
        self.formLayout.addRow("Sample rate [Hz]:", self.sampleRateLineEdit)

        # LED CURRENTS: BYTE 1..255 PLUS A LABEL WITH THE RESULTING mA
        self.ledGreenLineEdit, self.ledGreenMaLabel = self._add_led_row("Green LED (1-255):")
        self.ledIrLineEdit, self.ledIrMaLabel = self._add_led_row("IR LED (1-255):")
        self.ledRedLineEdit, self.ledRedMaLabel = self._add_led_row("Red LED (1-255):")

        self.ledRangeComboBox = QComboBox(form_box)
        self.formLayout.addRow("LED range:", self.ledRangeComboBox)
        self.tintComboBox = QComboBox(form_box)
        self.formLayout.addRow("Integration time:", self.tintComboBox)
        self.adcRangeComboBox = QComboBox(form_box)
        self.formLayout.addRow("ADC range:", self.adcRangeComboBox)
        self.sampleAvgComboBox = QComboBox(form_box)
        self.formLayout.addRow("Averaging:", self.sampleAvgComboBox)

        self.alcCheckBox = QCheckBox("Ambient light cancellation", form_box)
        self.formLayout.addRow("", self.alcCheckBox)
        self.proximityCheckBox = QCheckBox("Proximity check before start", form_box)
        self.formLayout.addRow("", self.proximityCheckBox)

        main_layout.addWidget(form_box)

        self.statusLabel = QLabel("Status: Ready", self)
        main_layout.addWidget(self.statusLabel)
        main_layout.addStretch(1)

        # TOOLTIPS INSTEAD OF THE HELP JSON OF THE WULPUS WIDGET
        self.sampleRateLineEdit.setToolTip(
            "Trigger rate of the nRF timer. Firmware needs >= 8 Hz.")
        for edit in (self.ledGreenLineEdit, self.ledIrLineEdit, self.ledRedLineEdit):
            edit.setToolTip(
                "Current = byte / 255 * LED range. Must be > 0: with 0 the "
                "firmware leaves this LED out of the packet and the decoding breaks.")
        self.sampleAvgComboBox.setToolTip(
            "Averaging inside the sensor. Leave at 1x unless tested: "
            "with averaging, the sensor may need several triggers per sample.")

    def _add_led_row(self, label: str) -> tuple[QLineEdit, QLabel]:
        row = QWidget(self)
        layout = QHBoxLayout(row)
        layout.setContentsMargins(0, 0, 0, 0)
        edit = QLineEdit(row)
        ma_label = QLabel("", row)
        ma_label.setMinimumWidth(70)
        layout.addWidget(edit, 1)
        layout.addWidget(ma_label)
        self.formLayout.addRow(label, row)
        return edit, ma_label

    def _setup_validators(self) -> None:
        self.sampleRateLineEdit.setValidator(QIntValidator(8, 255))
        for edit in (self.ledGreenLineEdit, self.ledIrLineEdit, self.ledRedLineEdit):
            edit.setValidator(QIntValidator(1, 255))

    def _populate_combo_boxes(self) -> None:
        # TEXT FOR THE USER, DATA = INDEX THE FIRMWARE GETS
        for i, ma in enumerate(LED_RANGE_MA):
            self.ledRangeComboBox.addItem(f"{ma} mA full scale", i)
        for i, us in enumerate(TINT_US):
            self.tintComboBox.addItem(f"{us} µs", i)
        for i, ua in enumerate(ADC_RANGE_UA):
            self.adcRangeComboBox.addItem(f"{ua} µA full scale", i)
        for i, n in enumerate(SAMPLE_AVG):
            self.sampleAvgComboBox.addItem(f"{n}x", i)

    def _connect_signals(self) -> None:
        self.presetComboBox.currentTextChanged.connect(self._on_preset_changed)
        self.saveConfigButton.clicked.connect(self._save_to_json)

        for edit in (
            self.sampleRateLineEdit,
            self.ledGreenLineEdit,
            self.ledIrLineEdit,
            self.ledRedLineEdit,
        ):
            edit.textChanged.connect(self._mark_as_custom)
        for combo in (
            self.ledRangeComboBox,
            self.tintComboBox,
            self.adcRangeComboBox,
            self.sampleAvgComboBox,
        ):
            combo.currentIndexChanged.connect(self._mark_as_custom)
        self.alcCheckBox.stateChanged.connect(self._mark_as_custom)
        self.proximityCheckBox.stateChanged.connect(self._mark_as_custom)

        # mA LABELS FOLLOW THE LED BYTES AND THE RANGE
        for edit in (self.ledGreenLineEdit, self.ledIrLineEdit, self.ledRedLineEdit):
            edit.textChanged.connect(self._update_ma_labels)
        self.ledRangeComboBox.currentIndexChanged.connect(self._update_ma_labels)

    def _update_ma_labels(self) -> None:
        range_ma = LED_RANGE_MA[self.ledRangeComboBox.currentData() or 0]
        for edit, label in (
            (self.ledGreenLineEdit, self.ledGreenMaLabel),
            (self.ledIrLineEdit, self.ledIrMaLabel),
            (self.ledRedLineEdit, self.ledRedMaLabel),
        ):
            try:
                label.setText(f"≈ {int(edit.text()) / 255 * range_ma:.1f} mA")
            except ValueError:
                label.setText("")

    # ── PRESETS (SAME LOGIC AS WulpusConfigWidget) ─────────────────────────
    def _get_presets_directory(self) -> Path:
        biogui_root = Path(__file__).parent.parent.parent.parent
        presets_dir = biogui_root / "presets" / "ppg"
        presets_dir.mkdir(parents=True, exist_ok=True)
        return presets_dir

    def _populate_presets(self) -> None:
        self.presetComboBox.blockSignals(True)
        self.presetComboBox.clear()
        self.presetComboBox.addItem("Custom")
        for preset_file in sorted(self._presets_dir.glob("*.json")):
            self.presetComboBox.addItem(preset_file.stem)
        self.presetComboBox.setCurrentText("Custom")
        self.presetComboBox.blockSignals(False)

    def _mark_as_custom(self) -> None:
        if self._loading_preset:
            return
        if self.presetComboBox.currentText() != "Custom":
            self.presetComboBox.blockSignals(True)
            self.presetComboBox.setCurrentText("Custom")
            self.presetComboBox.blockSignals(False)

    def _on_preset_changed(self, preset_name: str) -> None:
        if preset_name != "Custom":
            self._load_preset(preset_name)

    def _load_preset(self, preset_name: str) -> None:
        preset_file = self._presets_dir / f"{preset_name}.json"
        if not preset_file.exists():
            QMessageBox.warning(self, "Preset Not Found", f"'{preset_name}.json' not found.")
            return
        try:
            self._loading_preset = True
            with open(preset_file, "r") as file:
                self._apply_config_dict(json.load(file))
            self.statusLabel.setText(f"Status: Loaded preset '{preset_name}'")
        except Exception as err:
            QMessageBox.critical(self, "Load Error", f"Failed to load preset: {err}")
        finally:
            self._loading_preset = False

    def _save_to_json(self) -> None:
        file_path, _ = QFileDialog.getSaveFileName(
            self, "Save PPG Preset", str(self._presets_dir), "JSON Files (*.json)"
        )
        if not file_path:
            return
        if not file_path.endswith(".json"):
            file_path += ".json"
        try:
            with open(file_path, "w") as file:
                json.dump(self._get_config_dict(), file, indent=2)
            self.statusLabel.setText(f"Status: Saved to {Path(file_path).name}")
            if Path(file_path).parent == self._presets_dir:
                self._populate_presets()
                self.presetComboBox.blockSignals(True)
                self.presetComboBox.setCurrentText(Path(file_path).stem)
                self.presetComboBox.blockSignals(False)
        except Exception as err:
            QMessageBox.critical(self, "Save Error", f"Failed to save configuration: {err}")

    # ── CONFIG IN / OUT ───────────────────────────────────────────────────
    def load_config(self, config: PpgConfig) -> None:
        self._loading_preset = True
        try:
            self._apply_config_dict(asdict(config))
        finally:
            self._loading_preset = False

    def get_current_config(self) -> PpgConfig:
        """RAISES ValueError ON EMPTY OR OUT-OF-RANGE FIELDS."""
        return PpgConfig(**self._get_config_dict())

    def _get_config_dict(self) -> dict:
        rate = int(self.sampleRateLineEdit.text())
        leds = [
            int(self.ledGreenLineEdit.text()),
            int(self.ledIrLineEdit.text()),
            int(self.ledRedLineEdit.text()),
        ]
        # THE VALIDATOR ALLOWS HALF-TYPED VALUES, SO CHECK AGAIN HERE
        if not 8 <= rate <= 255:
            raise ValueError("PPG sample rate must be 8-255 Hz")
        if not all(1 <= v <= 255 for v in leds):
            raise ValueError("PPG LED currents must be 1-255")
        return {
            "sensor_mask": 0x01,  # ONE SENSOR, NO MUX
            "sample_rate_hz": rate,
            "led_green": leds[0],
            "led_ir": leds[1],
            "led_red": leds[2],
            "led_range": self.ledRangeComboBox.currentData(),
            "tint": self.tintComboBox.currentData(),
            "adc_range": self.adcRangeComboBox.currentData(),
            "sample_avg": self.sampleAvgComboBox.currentData(),
            "alc_enable": int(self.alcCheckBox.isChecked()),
            "proximity_enable": int(self.proximityCheckBox.isChecked()),
        }

    def _apply_config_dict(self, config_dict: dict) -> None:
        self.sampleRateLineEdit.setText(str(config_dict["sample_rate_hz"]))
        self.ledGreenLineEdit.setText(str(config_dict["led_green"]))
        self.ledIrLineEdit.setText(str(config_dict["led_ir"]))
        self.ledRedLineEdit.setText(str(config_dict["led_red"]))
        for combo, key in (
            (self.ledRangeComboBox, "led_range"),
            (self.tintComboBox, "tint"),
            (self.adcRangeComboBox, "adc_range"),
            (self.sampleAvgComboBox, "sample_avg"),
        ):
            idx = combo.findData(config_dict[key])
            if idx >= 0:
                combo.setCurrentIndex(idx)
        self.alcCheckBox.setChecked(bool(config_dict["alc_enable"]))
        self.proximityCheckBox.setChecked(bool(config_dict["proximity_enable"]))
        self._update_ma_labels()