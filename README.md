# ipspoof - Super Soviet Cybyer Spoofer

Здравствуйте, comdrade! If you would like to be just like a Russian SuperSpy™, this tool will allow you to send a UDP packet to any IP address with a spoofed IP packet header, effectively making it look like the traffic was coming from anywhere.

## Installation

```
git clone [REPO]
cd [REPO] && make docker
```

## Synopsis

```
ipspoof [SPOOF_IP] [TARGET_IP] [PAYLOAD]
```

## Example

Let's say, hypothetically, you wanted to send a packet to a site like `democrats.org` (52.84.26.91), from an address like the Kremlin, presumably at `kremlin.ru` (95.173.136.71) -

```
ipspoof 95.173.136.71 52.84.26.91 "hey it's totally us, the russians ... lulz"
```

This will then allow you to say that "it appears as if Russian cyber activity has occurred against the DNC's website, democrats.org".
