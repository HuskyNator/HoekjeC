![](afbeeldingen/Logo/Logo_Hoofd.png)

_HoekjeC is een grafische motor in aanbouw voor vermaak :)_

---

### Vereisten:
- gcc
- gdb

#### Kan via chocolatey geinstalleerd worden:
- Installeer chocolatey: https://chocolatey.org/
- `choco install mingw` (bevat gcc en gdb)

---

### Logo Aanpassen:
- Een `<iconaam>.ico` aanmaken (bij voorbeeld met paint.net addon).
- Een `<rcnaam>.rc` bestand de regel `id ICON "<iconaam>.ico"` geven.
- `windres \<rcnaam>.rc -o \<coffnaam>.coff`
- In task.json `<coffnaam>.coff` toevoegen als argument onder de te schakelen bestanden (zoals de c bestanden).

---
**Alle rechten voorbehouden.**<br>
_Voel je echter vrij in gesprek te gaan, vooral vermeld wegens gebrek aan licentiekeuze._
